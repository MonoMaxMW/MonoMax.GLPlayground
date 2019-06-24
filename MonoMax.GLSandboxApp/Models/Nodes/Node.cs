using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MonoMax.CPQ.Models.Nodes
{
    public abstract class Node
    {
        protected Node()
        {
            Name = GetType().Name;
        }

        protected readonly List<Node> _childNodes = new List<Node>();

        public string Name { get; set; }
        public object Tag { get; set; }
        public virtual bool CanBeDeleted => false;
        public virtual bool CanBeEdited => false;
        public Node Parent { get; internal set; }
        public IReadOnlyList<Node> ChildNodes => _childNodes;
    }
}
