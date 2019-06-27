using MonoMax.CPQ.Helpers;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace MonoMax.CPQ.Models.Nodes
{
    public sealed class NodeManager : List<Node>, INotifyPropertyChanged
    {
        static NodeManager _nodeManager;
        private RootNode _rootNode;
        private Node _selectedNode;
        private string _newName;

        public event PropertyChangedEventHandler PropertyChanged;

        public NodeManager()
        {
            Add(RootNode);

            Commands = new Dictionary<string, ICommand>()
            {
                {
                    "addNodeCmd",
                    new RelayCommand<Node>(
                        (node) =>
                        {
                            if(SelectedNode != null && SelectedNode.CanAddChildren)
                            {
                                SelectedNode.AddNode(new TextNode());
                                if(!SelectedNode.IsExpanded)
                                    SelectedNode.IsExpanded = true;
                            }
                        },
                        p => SelectedNode != null && SelectedNode.CanAddChildren,
                        "Add node")
                },
                {
                    "deleteNodeCmd",
                    new RelayCommand<Node>(
                        (node) =>
                        {
                            var parent = SelectedNode.Parent;
                            SelectedNode.DeleteNode();

                            SelectedNode = parent;
                            parent = null;
                        },
                        p => SelectedNode.CanBeEdited,
                        "Delete")
                },
                {
                    "changeNameCmd",
                    new RelayCommand<Node>(
                        (node) =>
                        {
                            SelectedNode.ChangeEditMode(!SelectedNode.IsInEditMode);
                        },
                        p => SelectedNode.CanBeEdited,
                        "Change name")
                },
            };
        }


        public string NewName
        {
            get { return _newName; }
            set
            {
                if(value != NewName)
                {
                    _newName = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(NewName)));
                }
            }
        }

        public RootNode RootNode
        {
            get
            {
                if (_rootNode == null)
                    _rootNode = new RootNode();

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

        public Node SelectedNode
        {
            get { return _selectedNode; }
            set
            {
                if(value != SelectedNode)
                {
                    SelectedNode?.ChangeEditMode(false);
                    _selectedNode = value;

                    if (SelectedNode != null)
                        NewName = SelectedNode.Name;

                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(SelectedNode)));
                }
            }
        }

        public IReadOnlyDictionary<string, ICommand> Commands { get; }
    }
}
