using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Windows.Forms;
using System.Text;

using IrrlichtLime;
using IrrlichtLime.Core;
using IrrlichtLime.GUI;
using IrrlichtLime.IO;
using IrrlichtLime.Scene;
using IrrlichtLime.Video;

namespace RubiksCube
{
    class Renderer
    {
        public IrrlichtDevice mDevice;
        public VideoDriver mDriver;
        public SceneManager mSceneMgr;
        public GUIEnvironment mGUIEnv;

        public Cube mCube;

        public CameraSceneNode mCamera;
        public Vec3 mCameraOffset;
        public Quat mCameraRotation;
        public Vec3 mCameraDirectionVec;
        public Vec3 mCameraUpVec;
        public bool mRotatingCamera = false;

        public Renderer(IntPtr windowHandle)
        {
            // Create the device
            IrrlichtCreationParameters parameters = new IrrlichtCreationParameters();
            parameters.DriverType = DriverType.OpenGL;
            parameters.WindowID = windowHandle;
            mDevice = IrrlichtDevice.CreateDevice(parameters);

            // Get interfaces
            mDriver = mDevice.VideoDriver;
            mSceneMgr = mDevice.SceneManager;
            mGUIEnv = mDevice.GUIEnvironment;

            // Create the camera
            mCamera = mSceneMgr.AddCameraSceneNode();
            mSceneMgr.ActiveCamera = mCamera;
            mSceneMgr.AmbientLight = new Colorf(1.0f, 1.0f, 1.0f);

            // Set the default camera position and rotation
            positionCamera(new Vec3(0.0f, 0.0f, -60.0f));
            rotateCamera(Quat.Identity);
        }

        public void setCube(Cube cube)
        {
            mCube = cube;
        }

        public void renderOnce()
        {
            mDriver.BeginScene(true, true, new Color(0, 40, 100));
            mSceneMgr.DrawAll();
            mGUIEnv.DrawAll();
            mDriver.EndScene();
            mDevice.Yield();
        }

        public void handleCommand(MainForm.Command command)
        {
            switch (command.Type)
            {
                case MainForm.CommandType.MouseMove:
                    {
                        if (mRotatingCamera)
                        {
                            Vector2Df move = (Vector2Df)command.Param;
                            float pixelsToRadians = 0.005f;
                            Quat rotateX = Quat.Rotate(move.Y * pixelsToRadians, new Vec3(1.0f, 0.0f, 0.0f));
                            Quat rotateY = Quat.Rotate(move.X * pixelsToRadians, new Vec3(0.0f, 1.0f, 0.0f));
                            rotateCamera(Quat.Mul(mCameraRotation, Quat.Mul(rotateX, rotateY)));
                        }
                        break;
                    }

                case MainForm.CommandType.MouseButtonDown:
                    {
                        MouseButtons button = (MouseButtons)command.Param;
                        if (button == MouseButtons.Left)
                            mRotatingCamera = true;
                        break;
                    }

                case MainForm.CommandType.MouseButtonUp:
                    {
                        MouseButtons button = (MouseButtons)command.Param;
                        if (button == MouseButtons.Left)
                            mRotatingCamera = false;
                        break;
                    }

                case MainForm.CommandType.TextCommand:
                    {
                        parseCommand((string)command.Param);
                        break;
                    }

                case MainForm.CommandType.Scramble:
                    {
                        // Initialise the RNG
                        Random random = new Random();

                        // Get a list of possible directions
                        Vec3[] directions = mCube.getAllPossibleDirections();

                        // Perform 20 random moves
                        for (int i = 0; i < 20; i++)
                            mCube.pushCommand(new Cube.Command(directions[random.Next(0, directions.Count() - 1)], (Cube.Rotation)random.Next(0, 1)));
                        break;
                    }

                case MainForm.CommandType.Undo:
                    {
                        mCube.undo();
                        break;
                    }

                case MainForm.CommandType.New:
                    {
                        mCube.reset();
                        break;
                    }

                case MainForm.CommandType.Open:
                    {
                        string[] lines;
                        try
                        {
                            lines = System.IO.File.ReadAllLines((string)command.Param);
                            if (lines.Count() < 27)
                                throw new Exception("Not enough lines");
                            mCube.deserialise(lines);
                        }
                        catch (Exception e)
                        {
                            MessageBox.Show("There was an error reading the cube file.\nError: " + e.Message);
                        }
                        break;
                    }

                case MainForm.CommandType.Save:
                    {
                        System.IO.TextWriter tw = null;
                        try
                        {
                            string[] lines = mCube.serialise();

                            if (mCube.executingMove())
                                throw new Exception("Cube is currently changing, cannot save now. Wait until it has finished then try again.");

                            // Write the output to the file
                            tw = new System.IO.StreamWriter((string)command.Param);
                            foreach (string l in lines)
                                tw.WriteLine(l);
                        }
                        catch (Exception e)
                        {
                            MessageBox.Show("There was an error writing to the cube file.\nError: " + e.Message);
                        }
                        if (tw != null)
                            tw.Close();
                        break;
                    }

                default:
                    break;
            }
        }

        public void positionCamera(Vec3 position)
        {
            mCameraOffset = position;

            // Update the position and rotation
            mCamera.Position = Vec3.ToIrr(position);
            mCamera.UpVector = new Vector3Df(0.0f, 1.0f, 0.0f);
            mCamera.Target = new Vector3Df(0.0f, 0.0f, 0.0f);
        }

        public void rotateCamera(Quat rotation)
        {
            mCameraRotation = rotation;

            // Calculate the camera position, up vector and target vector (for rotation)
            Vec3 newPosition = Vec3.Transform(mCameraOffset, rotation);
            Vec3 newUpVector = Vec3.Transform(new Vec3(0.0f, 1.0f, 0.0f), rotation);
            Vec3 newTargetVector = Vec3.Transform(new Vec3(0.0f, 0.0f, 1.0f), rotation);

            // Update the position and rotation
            mCamera.Position = Vec3.ToIrr(newPosition);
            mCamera.UpVector = Vec3.ToIrr(newUpVector);

            // Update the direction and up vectors
            mCameraDirectionVec = newTargetVector;
            mCameraUpVec = newUpVector;
        }

        public void parseCommand(string command)
        {
            // For the command to get this far, it's guaranteed to be valid
            // Command Syntax:
            // R = Rotate90 right
            // R' = RotateMinus90 right
            // L = Rotate90 left
            // L' = RotateMinus90 left
            // U = Rotate90 top
            // U' = RotateMinus90 top
            // D = Rotate90 down
            // D' = RotateMinus90 down
            // F = Rotate90 front
            // F' = RotateMinus90 front
            // B = Rotate90 back
            // B' = RotateMinus90 back

            // Split the string on every space into sub strings
            string[] commands = command.Split(' ');

            Vec3 direction = mCameraDirectionVec;
            Vec3 top = mCameraUpVec;

            // Create a list of all directions
            Vec3[] directions = mCube.getAllPossibleDirections();

            // Compare each direction with the two vectors, and set them to the index with the greatest dot product
            // Greater dot product = most similar
            int closestDirVecIndex = -1;
            float closestDirVecDP = -1.0f;
            int closestTopVecIndex = -1;
            float closestTopVecDP = -1.0f;
            for (int i = 0; i < 6; i++)
            {
                // Direction
                float dirVecDP = Vec3.Dot(direction, directions[i]);
                if (dirVecDP > closestDirVecDP)
                {
                    closestDirVecIndex = i;
                    closestDirVecDP = dirVecDP;
                }

                // Top
                float topVecDP = Vec3.Dot(top, directions[i]);
                if (topVecDP > closestTopVecDP)
                {
                    closestTopVecIndex = i;
                    closestTopVecDP = topVecDP;
                }
            }

            // Set the direction and top vectors
            direction = directions[closestDirVecIndex];
            top = directions[closestTopVecIndex];

            // Select the axis of rotation
            // Back = Direction Vector
            // Up = Top Vector
            // Right = Cross( Direction, Top )
            // Front = -Back
            // Down = -Top
            // Left = Cross( Top, Direction )
            foreach (string c in commands)
            {
                switch (c)
                {
                    case "B":
                        mCube.pushCommand(new Cube.Command(direction, Cube.Rotation.Rotate90));
                        break;

                    case "B'":
                        mCube.pushCommand(new Cube.Command(direction, Cube.Rotation.RotateMinus90));
                        break;

                    case "U":
                        mCube.pushCommand(new Cube.Command(top, Cube.Rotation.Rotate90));
                        break;

                    case "U'":
                        mCube.pushCommand(new Cube.Command(top, Cube.Rotation.RotateMinus90));
                        break;

                    case "R":
                        mCube.pushCommand(new Cube.Command(Vec3.Cross(top, direction), Cube.Rotation.Rotate90));
                        break;

                    case "R'":
                        mCube.pushCommand(new Cube.Command(Vec3.Cross(top, direction), Cube.Rotation.RotateMinus90));
                        break;

                    case "F":
                        mCube.pushCommand(new Cube.Command(new Vec3(-direction.x, -direction.y, -direction.z), Cube.Rotation.Rotate90));
                        break;

                    case "F'":
                        mCube.pushCommand(new Cube.Command(new Vec3(-direction.x, -direction.y, -direction.z), Cube.Rotation.RotateMinus90));
                        break;

                    case "D":
                        mCube.pushCommand(new Cube.Command(new Vec3(-top.x, -top.y, -top.z), Cube.Rotation.Rotate90));
                        break;

                    case "D'":
                        mCube.pushCommand(new Cube.Command(new Vec3(-top.x, -top.y, -top.z), Cube.Rotation.RotateMinus90));
                        break;

                    case "L":
                        mCube.pushCommand(new Cube.Command(Vec3.Cross(direction, top), Cube.Rotation.Rotate90));
                        break;

                    case "L'":
                        mCube.pushCommand(new Cube.Command(Vec3.Cross(direction, top), Cube.Rotation.RotateMinus90));
                        break;

                    default:
                        break;
                }
            }
        }

        public int getFPS()
        {
            return mDevice.VideoDriver.FPS;
        }
    }
}