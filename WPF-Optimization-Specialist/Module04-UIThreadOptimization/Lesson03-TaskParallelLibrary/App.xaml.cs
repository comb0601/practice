using System.Windows;

namespace TaskParallelLibrary;

public partial class App : Application
{
    protected override void OnStartup(StartupEventArgs e)
    {
        base.OnStartup(e);

        // Configure task scheduler exception handling
        TaskScheduler.UnobservedTaskException += (sender, args) =>
        {
            MessageBox.Show($"Unobserved Task Exception: {args.Exception.Message}",
                "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            args.SetObserved();
        };

        this.DispatcherUnhandledException += (sender, args) =>
        {
            MessageBox.Show($"Dispatcher Exception: {args.Exception.Message}",
                "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            args.Handled = true;
        };
    }
}
