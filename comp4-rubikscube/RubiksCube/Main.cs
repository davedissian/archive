using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Text.RegularExpressions;

using IrrlichtLime;
using IrrlichtLime.Core;
using IrrlichtLime.GUI;
using IrrlichtLime.IO;
using IrrlichtLime.Scene;
using IrrlichtLime.Video;

namespace RubiksCube
{
    public partial class MainForm : Form
    {
        Thread mRenderThread;
        Queue<Command> mCommandQueue;
        Vector2Df mCurrentCursorPosition;

        string mFilename = null;
        bool mModified = false;

        public enum CommandType
        {
            MouseButtonDown,
            MouseButtonUp,
            MouseMove,
            TextCommand,
            Scramble,
            Undo,

            // File operations
            New,
            Open,
            Save
        }

        public class Command
        {
            public CommandType Type;
            public object Param;
        }

        public MainForm()
        {
            InitializeComponent();
            updateTitleBar();
        }

        private void appThreadMain(object arg)
        {
            Renderer renderer = new Renderer((IntPtr)arg);
            Cube cube = new Cube(renderer);
            renderer.setCube(cube);
            try
            {
                while (renderer.mDevice.Run())
                {
                    cube.update();
                    renderer.renderOnce();

                    // Process all commands in the queue
                    Command c = null;
                    bool hasAnotherCommand = true;
                    while (hasAnotherCommand)
                    {
                        lock (mCommandQueue)
                        {
                            if (mCommandQueue.Count() > 0)
                            {
                                hasAnotherCommand = true;
                                c = mCommandQueue.Dequeue();
                            }
                            else
                            {
                                hasAnotherCommand = false;
                                c = null;
                            }
                        }
                        if (c != null)
                            renderer.handleCommand(c);
                    }
                }
            }
            catch (ThreadAbortException)
            {
                renderer.mDevice.Close();
            }
        }

        private void updateTitleBar()
        {
            string titleBar = "Rubik's Cube - ";

            // Current Filename
            if (mFilename != null)
                titleBar += mFilename;
            else
                titleBar += "<unnamed>";

            // Is it modified?
            if (mModified)
                titleBar += "*";

            // Update title bar
            Text = titleBar;
        }

        private void queueCommand(CommandType type, object param)
        {
            Command c = new Command();
            c.Type = type;
            c.Param = param;

            lock (mCommandQueue)
            {
                mCommandQueue.Enqueue(c);
            }
        }

        private void Main_Load(object sender, EventArgs e)
        {
            // Start the application thread
            mCommandQueue = new Queue<Command>();
            mRenderThread = new Thread(new ParameterizedThreadStart(appThreadMain));
            mRenderThread.Start(renderPanel.Handle);
        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Stop the application thread
            mRenderThread.Abort();
        }

        private void renderPanel_MouseMove(object sender, MouseEventArgs e)
        {
            Vector2Df newCursorPosition = new Vector2Df((float)e.X, (float)e.Y);
            if (mCurrentCursorPosition != null)
                queueCommand(CommandType.MouseMove, mCurrentCursorPosition - newCursorPosition);
            mCurrentCursorPosition = newCursorPosition;
        }

        private void renderPanel_MouseDown(object sender, MouseEventArgs e)
        {
            queueCommand(CommandType.MouseButtonDown, e.Button);
        }

        private void renderPanel_MouseUp(object sender, MouseEventArgs e)
        {
            queueCommand(CommandType.MouseButtonUp, e.Button);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void insertCommandsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Get the input - eliminating duplicate spaces
            frmInsertCmd insertCommandForm = new frmInsertCmd();
            insertCommandForm.ShowDialog();
            string input = Regex.Replace(insertCommandForm.getInput().Trim(), @"\s+", " ");

            // Ignore the command if it's empty
            if (input.Length == 0)
                return;

            // Validate the input with a regular expression
            Regex r = new Regex("^([FBUDLR]'?\\s)*([FBUDLR]'?)$");
            if (r.IsMatch(input))
            {
                queueCommand(CommandType.TextCommand, input);
                mModified = true;
                updateTitleBar();
            }
            else
            {
                MessageBox.Show("Sorry, the expression entered isn't valid.");
            }
        }

        private void scrambleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            queueCommand(CommandType.Scramble, null);
            mModified = true;
            updateTitleBar();
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            queueCommand(CommandType.Undo, null);
            mModified = true;
            updateTitleBar();
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            queueCommand(CommandType.New, null);
            mFilename = null;
            mModified = false;
            updateTitleBar();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Open an open dialog and pass the file name to the cube thread
            openCube.FileName = "";
            openCube.Title = "Open a previously saved cube";
            openCube.Filter = "Rubik's Cube|*.cube";
            if (openCube.ShowDialog() != DialogResult.Cancel)
            {
                queueCommand(CommandType.Open, openCube.FileName);
                mFilename = openCube.FileName;
                mModified = false;
                updateTitleBar();
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Open a save dialog if we've never saved before
            if (mFilename == null)
            {
                saveAsToolStripMenuItem_Click(sender, e);
            }
            else
            {
                // Overwrite the previous file
                queueCommand(CommandType.Save, mFilename);
                mModified = false;
                updateTitleBar();
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Open a save dialog and pass the file name to the cube thread
            saveCube.FileName = "";
            saveCube.Title = "Save a cube to a file";
            saveCube.Filter = "Rubik's Cube|*.cube";
            if (saveCube.ShowDialog() != DialogResult.Cancel && saveCube.FileName != "")
            {
                queueCommand(CommandType.Save, saveCube.FileName);
                mFilename = saveCube.FileName;
                mModified = false;
                updateTitleBar();
            }
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            frmAbout about = new frmAbout();
            about.ShowDialog();
        }
    }
}