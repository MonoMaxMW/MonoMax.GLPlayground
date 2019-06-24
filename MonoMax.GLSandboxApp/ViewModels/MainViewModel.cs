using Caliburn.Micro;
using MonoMax.CPQ.ViewModels.Documents;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MonoMax.CPQ.ViewModels
{
    public interface IMainView { }

    [Export(typeof(IMainView))]
    public class MainViewModel : PropertyChangedBase, IMainView
    {
        private IPanel _activePanel;

        [ImportingConstructor]
        public MainViewModel([ImportMany] IEnumerable<IPanel> panels)
        {
            Panels = panels?.ToList();
            SceneViewModel = Panels.FirstOrDefault(x => x.Caption == nameof(SceneViewModel));
            NodesViewModel = Panels.FirstOrDefault(x => x.Caption == nameof(NodesViewModel));
        }

        public IPanel SceneViewModel { get; }
        public IPanel NodesViewModel { get; }

        public IReadOnlyList<IPanel> Panels { get; }
    }
}
