using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using MonoMax.CPQ.Helpers;
using MonoMaxGraphics; 

namespace MonoMax.CPQ.ViewModels.Documents
{
    public class SceneViewModel : PanelViewModel, IDocument
    {
        public SceneViewModel()
        {
            LoadMockDataCmd = new RelayCommand<object>(
                (o) =>
                {
                    GfxFramework.AddMesh(@"C:\tmp\stl\test_1.stl");


                });
        }

        public GraphicFramework GfxFramework { get; internal set; }

        public ICommand LoadMockDataCmd { get; }

    }
}
