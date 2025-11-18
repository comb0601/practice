using System.Windows;

namespace BackgroundWorkers;

public partial class App : Application
{
    protected override void OnStartup(StartupEventArgs e)
    {
        base.OnStartup(e);

        // Global exception handlers
        this.DispatcherUnhandledException += (sender, args) =>
        {
            MessageBox.Show($"UI Thread Exception: {args.Exception.Message}",
                "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            args.Handled = true;
        };

        AppDomain.CurrentDomain.UnhandledException += (sender, args) =>
        {
            var exception = args.ExceptionObject as Exception;
            MessageBox.Show($"Unhandled Exception: {exception?.Message}",
                "Critical Error", MessageBoxButton.OK, MessageBoxImage.Error);
        };
    }
}
