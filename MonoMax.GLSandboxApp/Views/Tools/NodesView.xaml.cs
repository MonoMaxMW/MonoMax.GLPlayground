using MonoMax.CPQ.Models.Nodes;
using MonoMax.CPQ.ViewModels.Tools;
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

namespace MonoMax.CPQ.Views.Tools
{
    /// <summary>
    /// Interaction logic for NodesView.xaml
    /// </summary>
    public partial class NodesView : UserControl
    {
        public NodesView()
        {
            InitializeComponent();
        }

        private void Treeview_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            var vm = (this.DataContext as NodesViewModel);
            var item = e.NewValue as Node;

            vm.NodeManager.SelectedNode = item;

            item = null;
            vm = null;
        }

        private void TextBox_Loaded(object sender, RoutedEventArgs e)
        {
            var tb = sender as TextBox;

            tb.Focus();
            tb.SelectAll();

        }

        private void TextBox_Unloaded(object sender, RoutedEventArgs e)
        {
            treeview.Items.Refresh();
        }
    }
}
