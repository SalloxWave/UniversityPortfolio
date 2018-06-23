using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using Ludo.Engine.alexmickeversion;
using Ludo.Data.alexmickeversion;

namespace Ludo
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        void App_Startup(object sender, StartupEventArgs e)
        {            
            // Application is running
            // Process command line args
            bool startMinimized = false;
            for (int i = 0; i != e.Args.Length; ++i)
            {
                if (e.Args[i] == "/StartMinimized")
                {
                    startMinimized = true;
                }
            }
            LudoGUI mainWindow = null;
            try
            {
                // Create main application window
                mainWindow = new LudoGUI(new LudoEngine(new LudoRules(), new LudoData(), 4));
            }
            catch(Exception except)
            {
                MessageBoxResult result = 
                    MessageBox.Show("Could not start game. Would you like to reset game files?", 
                    "Failed to start game (" + except.HResult + ")", MessageBoxButton.YesNoCancel, MessageBoxImage.Error);
                
                if (result == MessageBoxResult.Yes)
                {
                    LudoData.Delete();
                    this.OnStartup(e);
                    return;
                }
                else
                    Environment.Exit(1);
            }            

            if (startMinimized)
            {
                mainWindow.WindowState = WindowState.Minimized;
            }
            mainWindow.Show();
        }
    }
}
