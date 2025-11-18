using System.Windows;

namespace AsyncPatterns;

public partial class App : Application
{
    protected override void OnStartup(StartupEventArgs e)
    {
        base.OnStartup(e);

        // Configure default exception handling for async operations
        this.DispatcherUnhandledException += (sender, args) =>
        {
            MessageBox.Show($"Unhandled exception: {args.Exception.Message}",
                "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            args.Handled = true;
        };

        TaskScheduler.UnobservedTaskException += (sender, args) =>
        {
            MessageBox.Show($"Unobserved task exception: {args.Exception.Message}",
                "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            args.SetObserved();
        };
    }
}
