using MonoMax.CPQ.Models.Nodes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MonoMax.CPQ.ViewModels.Tools
{
    public class NodesViewModel : PanelViewModel
    {
        public NodesViewModel()
        {
            //var n = NodeManager.Instance;
            //NotifyOfPropertyChange(() => NodeManger);
        }

        public NodeManager NodeManager => NodeManager.Instance;
    }
}
