using MonoMax.CPQ.Models.Nodes;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media;

namespace MonoMax.CPQ.Helpers
{
    public sealed class NodeTypeBackgroundConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var colorBrush = new SolidColorBrush(Colors.Black);
            var node = value as Node;

            if (node != null)
            {
                switch (node.NodeType)
                {
                    case NodeType.Root:
                        colorBrush = new SolidColorBrush(Colors.DarkSlateGray);
                        break;
                    case NodeType.Text:
                        colorBrush = new SolidColorBrush(Colors.LightCoral);
                        break;
                    case NodeType.Mesh:
                        colorBrush = new SolidColorBrush(Colors.SaddleBrown);
                        break;
                    case NodeType.Transformation:
                        break;
                } 
            }

            return colorBrush;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
