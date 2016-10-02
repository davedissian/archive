using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RubiksCube
{
    class Cube
    {
        public enum Rotation
        {
            Rotate90,
            RotateMinus90
        }

        public class Command
        {
            public Vec3 Slice;
            public Rotation Rotation;

            public Command(Vec3 _slice, Rotation _rotation)
            {
                Slice = _slice;
                Rotation = _rotation;
            }
        }

        public Cube(Renderer renderer)
        {
            mRenderer = renderer;

            // Populate the cube pieces array
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    for (int z = 0; z < 3; z++)
                    {
                        if (!(x == 1 && y == 1 && z == 1))
                        {
                            mPieces.Add(new CubePiece(new Vec3(x - 1, y - 1, z - 1), Quat.Identity, mRenderer, 1));
                        }
                    }
                }
            }
        }

        public void reset()
        {
            mCommandQueue.Clear();
            mCommandHistory.Clear();
            mCurrentCommand = null;
            mCurrentSlice.Clear();
            mCurrentCommandStep = 0;
            mCurrentCommandStepTransform = Quat.Identity;
            mCurrentCommandAxis = null;

            // Reset each cube
            int count = 0;
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    for (int z = 0; z < 3; z++)
                    {
                        if (!(x == 1 && y == 1 && z == 1))
                        {
                            mPieces[count].Position = new Vec3(x - 1, y - 1, z - 1);
                            mPieces[count].Rotation = Quat.Identity;
                            count++;
                        }
                    }
                }
            }
        }

        public void deserialise(string[] lines)
        {
            mCommandQueue.Clear();
            mCommandHistory.Clear();
            mCurrentCommand = null;
            mCurrentSlice.Clear();
            mCurrentCommandStep = 0;
            mCurrentCommandStepTransform = Quat.Identity;
            mCurrentCommandAxis = null;

            // Set each cube piece (first 26 lines)
            // Format: x y z vx vy vz s
            for (int i = 0; i < 26; i++)
            {
                string[] tokens = lines[i].Split(' ');
                Vec3 position = new Vec3();
                Quat rotation = new Quat();

                // Parse the tokens and set the piece transforms
                float.TryParse(tokens[0], out position.x);
                float.TryParse(tokens[1], out position.y);
                float.TryParse(tokens[2], out position.z);
                float.TryParse(tokens[3], out rotation.v.x);
                float.TryParse(tokens[4], out rotation.v.y);
                float.TryParse(tokens[5], out rotation.v.z);
                float.TryParse(tokens[6], out rotation.s);
                mPieces[i].Position = position;
                mPieces[i].Rotation = rotation;
            }

            // Add each command to the command history
            // Format: x y z 0/1
            int noCommands;
            int.TryParse(lines[26], out noCommands);
            if (noCommands > 0)
            {
                for (int i = 27; i < (27 + noCommands); i++)
                {
                    string[] tokens = lines[i].Split(' ');
                    Command command = new Command(new Vec3(), Rotation.Rotate90);
                    int rotationInt;

                    // Parse the tokens and extract each command
                    float.TryParse(tokens[0], out command.Slice.x);
                    float.TryParse(tokens[1], out command.Slice.y);
                    float.TryParse(tokens[2], out command.Slice.z);
                    int.TryParse(tokens[3], out rotationInt);
                    if (rotationInt == 0)
                        command.Rotation = Rotation.Rotate90;
                    else
                        command.Rotation = Rotation.RotateMinus90;

                    // Add the command to the history
                    mCommandHistory.Add(command);
                }
            }
        }

        public string[] serialise()
        {
            List<string> output = new List<string>();

            // Write the current cube state
            for (int i = 0; i < 26; i++)
                output.Add(string.Format("{0} {1} {2} {3} {4} {5} {6}",
                    mPieces[i].Position.x, mPieces[i].Position.y, mPieces[i].Position.z,
                    mPieces[i].Rotation.v.x, mPieces[i].Rotation.v.y, mPieces[i].Rotation.v.z, mPieces[i].Rotation.s));

            // Write the move history
            output.Add(string.Format("{0}", mCommandHistory.Count));
            for (int i = 0; i < mCommandHistory.Count(); i++)
            {
                Command c = mCommandHistory[i];
                output.Add(string.Format("{0} {1} {2} {3}", c.Slice.x, c.Slice.y, c.Slice.z, c.Rotation == Rotation.Rotate90 ? 0 : 1));
            }

            // Convert to a raw array and return
            string[] lines = new string[output.Count()];
            for (int i = 0; i < output.Count(); i++)
                lines[i] = output[i];
            return lines;
        }

        public void pushCommand(Command command, bool addToHistory = true)
        {
            mCommandQueue.Enqueue(command);
            if (addToHistory)
                mCommandHistory.Add(command);
        }

        public void update()
        {
            if (mCurrentCommand == null)
            {
                // If theres no command currently being executed, then move to the next one
                if (mCommandQueue.Count > 0)
                {
                    mCurrentCommand = mCommandQueue.Dequeue();
                    changeCommand();
                }
            }
            else
            {
                // Perform one step of the command currently being executed
                if (mCurrentCommandStep < mNoSteps)
                {
                    mCurrentCommandStep++;
                    updateCube();
                }
                else
                {
                    mCurrentCommand = null;
                    mCurrentCommandStep = 0;
                    fixCubePieces();
                }
            }
        }

        public bool executingMove()
        {
            return mCurrentCommand != null;
        }

        public void changeCommand()
        {
            // Get the rotation around the specified axis
            float angle;
            switch (mCurrentCommand.Rotation)
            {
                case Rotation.Rotate90:
                    angle = (float)Math.PI * -0.5f;
                    break;

                case Rotation.RotateMinus90:
                    angle = (float)Math.PI * 0.5f;
                    break;

                default:
                    return;
            }

            // Construct the quaternion
            mCurrentCommandAxis = mCurrentCommand.Slice;
            mCurrentCommandStep = 0;
            mNoSteps = mRenderer.getFPS() * 2;
            mCurrentCommandStepTransform = Quat.Rotate(angle / (float)mNoSteps, mCurrentCommandAxis);

            // Select all the cubes in this slice
            mCurrentSlice.Clear();
            foreach (CubePiece piece in mPieces)
            {
                if (mCurrentCommandAxis.x != 0.0f)
                {
                    if (piece.Position.x == mCurrentCommandAxis.x)
                        mCurrentSlice.Add(piece);
                }
                else if (mCurrentCommandAxis.y != 0.0f)
                {
                    if (piece.Position.y == mCurrentCommandAxis.y)
                        mCurrentSlice.Add(piece);
                }
                else if (mCurrentCommandAxis.z != 0.0f)
                {
                    if (piece.Position.z == mCurrentCommandAxis.z)
                        mCurrentSlice.Add(piece);
                }
            }
        }

        public void updateCube()
        {
            // Transform each piece by "rotate"
            foreach (CubePiece piece in mCurrentSlice)
            {
                piece.Position = Vec3.Transform(piece.Position, mCurrentCommandStepTransform);

                // Normalise the quaternion to prevent it from "degrading"
                piece.Rotation = Quat.Normalise(Quat.Mul(piece.Rotation, Quat.Conjugate(mCurrentCommandStepTransform)));
            }
        }

        public void fixCubePieces()
        {
            // When being rotated by quaternions, the cube piece positions are always a tiny bit out, so fix it here.
            foreach (CubePiece piece in mPieces)
            {
                Vec3 fixedPosition = piece.Position;
                fixedPosition.x = (float)Math.Round(fixedPosition.x);
                fixedPosition.y = (float)Math.Round(fixedPosition.y);
                fixedPosition.z = (float)Math.Round(fixedPosition.z);
                piece.Position = fixedPosition;
            }
        }

        public void undo()
        {
            if (mCommandHistory.Count > 0)
            {
                // Generate a command which reverses the previous one
                Command reversedCommand = mCommandHistory[mCommandHistory.Count() - 1];
                mCommandHistory.Remove(reversedCommand);
                if (reversedCommand.Rotation == Rotation.Rotate90)
                    reversedCommand.Rotation = Rotation.RotateMinus90;
                else
                    reversedCommand.Rotation = Rotation.Rotate90;

                // Perform this action
                pushCommand(reversedCommand, false);
            }
        }

        public Vec3[] getAllPossibleDirections()
        {
            Vec3[] directions = new Vec3[6];
            directions[0] = new Vec3(0.0f, 0.0f, 1.0f);
            directions[1] = new Vec3(0.0f, 0.0f, -1.0f);
            directions[2] = new Vec3(0.0f, 1.0f, 0.0f);
            directions[3] = new Vec3(0.0f, -1.0f, 0.0f);
            directions[4] = new Vec3(1.0f, 0.0f, 0.0f);
            directions[5] = new Vec3(-1.0f, 0.0f, 0.0f);
            return directions;
        }

        private Renderer mRenderer = null;
        private List<CubePiece> mPieces = new List<CubePiece>(26);

        // Command Queue and History
        private Queue<Command> mCommandQueue = new Queue<Command>();
        private List<Command> mCommandHistory = new List<Command>();

        // Current Command being executed
        private Command mCurrentCommand = null;
        private List<CubePiece> mCurrentSlice = new List<CubePiece>();
        private int mNoSteps = 100;
        private int mCurrentCommandStep = 0;
        private Quat mCurrentCommandStepTransform = Quat.Identity;
        private Vec3 mCurrentCommandAxis = null;
    }
}