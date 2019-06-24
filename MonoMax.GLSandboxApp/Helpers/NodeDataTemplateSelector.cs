using MonoMax.CPQ.Models.Nodes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace MonoMax.CPQ.Helpers
{
    public sealed class NodeDataTemplateSelector : DataTemplateSelector
    {
        public DataTemplate NodeTemplate { get; set; }

        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            if (item is Node)
                return NodeTemplate;

            return base.SelectTemplate(item, container);
        }
    }
}
