using Caliburn.Micro;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace MonoMax.CPQ.Models.Nodes
{
    public enum NodeType
    {
        Root,
        Text,
        Mesh,
        Transformation
    }

    public abstract class Node : PropertyChangedBase
    {
        private bool _isExpanded;
        private bool _isSelected;
        protected readonly ObservableCollection<Node> _childNodes = new ObservableCollection<Node>();

        protected Node()
        {
            Name = GetType().Name;
        }

        public abstract NodeType NodeType { get; }
        public object Tag { get; set; }
        public string Name { get; protected set; }
        public Node Parent { get; private set; }
        public virtual bool CanBeDeleted => false;
        public virtual bool CanBeEdited => false;
        public virtual bool CanAddChildren => true;
        public IReadOnlyCollection<Node> ChildNodes => _childNodes;

        public bool IsInEditMode { get; private set; }

        public bool IsSelected
        {
            get { return _isSelected; }
            set
            {
                _isSelected = value;
                NotifyOfPropertyChange();
            }
        }


        public void ChangeName(string name)
        {
            Name = name;
        }

        public void ChangeEditMode(bool editable)
        {
            if (editable == IsInEditMode)
                return;

            IsInEditMode = editable;
            NotifyOfPropertyChange(() => IsInEditMode);
        }

        protected virtual void BeforeAdd()
        {

        }

        protected virtual void  BeforeDelete()
        {

        }

        public void AddNode(Node node)
        {
            node.Parent = this;

            BeforeAdd();
            _childNodes.Add(node);
        }

        public void DeleteNode()
        {
            Parent.RemoveNode(this);
        }

        public void RemoveNode(Node node)
        {
            if (_childNodes.Count > 0)
            {
                foreach (var n in _childNodes)
                {
                    if(n == node)
                    {
                        BeforeDelete();
                        _childNodes.Remove(node);
                        break;
                    }
                } 
            }
        }

        public bool IsExpanded
        {
            get { return _isExpanded; }
            set
            {
                if (value != IsExpanded)
                {
                    _isExpanded = value;
                    NotifyOfPropertyChange();
                }
            }
        }
    }
}
