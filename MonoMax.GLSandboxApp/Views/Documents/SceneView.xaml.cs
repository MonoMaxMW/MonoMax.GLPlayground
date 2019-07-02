using Caliburn.Micro;
using MonoMax.CPQ.ViewModels.Documents;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace MonoMax.CPQ.Views.Documents
{
    /// <summary>
    /// Interaction logic for SceneView.xaml
    /// </summary>
    public partial class SceneView : UserControl
    {
        public SceneView()
        {
            InitializeComponent();
        }

        private void GLControl_Initialized(object sender, EventArgs e)
        {
            var vm = DataContext as SceneViewModel;

            if (vm != null)
            {
                vm.GfxFramework = GLControl.GetGfxFramework();
            }

        }

        private void GLControl_Unloaded(object sender, RoutedEventArgs e)
        {

        }
    }
}
