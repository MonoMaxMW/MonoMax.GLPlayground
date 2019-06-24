using Caliburn.Micro;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MonoMax.CPQ.ViewModels
{
    [InheritedExport(typeof(IPanel))]
    public abstract class PanelViewModel : PropertyChangedBase, IPanel
    {
        public virtual string Caption => GetType().Name;
        public virtual bool CanFloat => true;
        public virtual bool CanClose => false;

        public virtual void Activate()
        {

        }

        public virtual void Deactivate()
        {

        }
    }
}
