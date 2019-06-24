using MonoMax.CPQ.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace MonoMax.CPQ.Helpers
{
    public sealed class AutobinderTemplateSelector : DataTemplateSelector
    {
        public DataTemplate Template { get; set; }

        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            if (item is IPanel)
                return Template;

            return base.SelectTemplate(item, container);
        }

    }
}
