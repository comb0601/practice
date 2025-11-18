using System.Windows;

namespace UIVirtualization;

public partial class App : Application
{
    protected override void OnStartup(StartupEventArgs e)
    {
        base.OnStartup(e);

        this.DispatcherUnhandledException += (sender, args) =>
        {
            MessageBox.Show($"Application Error: {args.Exception.Message}",
                "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            args.Handled = true;
        };
    }
}
