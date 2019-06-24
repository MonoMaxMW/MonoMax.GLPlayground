using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MonoMax.CPQ.ViewModels
{
    public interface IPanel
    {
        string Caption { get; }
        bool CanFloat { get; }
        bool CanClose { get; }
        void Activate();
        void Deactivate();
    }
}
