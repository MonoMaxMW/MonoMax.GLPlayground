using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MonoMax.CPQ.Models.Nodes
{
    public sealed class NodeManager : List<Node>
    {
        static NodeManager _nodeManager;
        private RootNode _rootNode;

        public NodeManager()
        {
            Add(RootNode);
        }

        public RootNode RootNode
        {
            get
            {
                if (_rootNode == null)
                    _rootNode = new RootNode() { Name = "Root" };

                return _rootNode;
            }
        }

        public static NodeManager Instance
        {
            get
            {
                if (_nodeManager == null)
                    _nodeManager = new NodeManager();

                return _nodeManager;
            }
        }
    }
}
