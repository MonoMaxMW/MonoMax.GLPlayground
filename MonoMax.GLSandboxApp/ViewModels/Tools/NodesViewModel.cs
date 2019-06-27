using MonoMax.CPQ.Models.Nodes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace MonoMax.CPQ.ViewModels.Tools
{
    public class NodesViewModel : PanelViewModel
    {
        public NodesViewModel()
        {
            //var n = NodeManager.Instance;
            //NotifyOfPropertyChange(() => NodeManger);
        }

        public void EditableTextboxKeyPressHandler(KeyEventArgs keyEventArgs)
        {
            if(keyEventArgs.Key == Key.Enter || keyEventArgs.Key == Key.F2)
            {
                if (!string.IsNullOrEmpty(NodeManager.NewName))
                {
                    NodeManager.SelectedNode.ChangeName(NodeManager.NewName);
                }

                NodeManager.SelectedNode.ChangeEditMode(false);
            }
            else if(keyEventArgs.Key == Key.Escape)
            {
                NodeManager.SelectedNode.ChangeEditMode(false);
            }
        }

        public NodeManager NodeManager => NodeManager.Instance;
    }
}
