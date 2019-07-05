using System;
using System.Collections.Generic;
using System.IO;
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
                    string[] files = Directory.GetFiles(@"C:\tmp\machines\VT260", "*.stl");

                    if(files != null && files.Length > 0)
                    {
                        for (int i = 0; i < files.Length; i++)
                        {
                            GfxFramework.AddMesh(files[i]);

                        }
                    }



                });
        }

        public GraphicFramework GfxFramework { get; internal set; }

        public ICommand LoadMockDataCmd { get; }

    }
}
