using Editor.GameProject;
using Editor.Utilities;
using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Editor.GameDevelopment
{
    public partial class CreateScriptDialog : Window
    {
        private static readonly string _cppCode = 
            @"#include ""{0}.h""

namespace {1} {{

REGISTER_SCRIPT({{0}});


void {0}::BeginPlay()
{{

}}


void {0}::Update(float dt)
{{

}}

}} // namespace {1}";

        private static readonly string _hCode =
            @"#pragma once

namespace {1} {{

class {0} : public geENTITY_SCRIPT
{{
    constexpr explicit {0}(geENTITY entity)
        : geENTITY_SCRIPT(entity)
    {{

    }}

    void BeginPlay() override;
    void Update(float dt) override;
}};

}} // namespace {1}";

        private static readonly string _namespace = GetNamespaceFromProjectName();

        public CreateScriptDialog()
        {
            InitializeComponent();
            Owner = Application.Current.MainWindow;
            scriptPath.Text = @"GameCode\";
        }

        private void OnScriptNameTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (!Validate())
            {
                return;
            }

            var name = scriptName.Text.Trim();
            messageTextBlock.Text = $"{name}.h and {name}.cpp will be added to {Project.CurrentGameProject!.Name}";
        }

        private void OnScriptPathTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            Validate();
        }

        private async void OnCreateButton_Click(object sender, RoutedEventArgs e)
        {
            if (!Validate())
            {
                return;
            }

            IsEnabled = false;

            try
            {
                var name = scriptName.Text;
                var path = Path.GetFullPath(Path.Combine(Project.CurrentGameProject!.Path, scriptPath.Text.Trim()));
                var solution = Project.CurrentGameProject.Solution;
                var projectName = Project.CurrentGameProject.Name;


                await Task.Run(() => CreateScript(name, path, solution, projectName));
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, $"Failed to create script {scriptName.Text}");
            }
        }

        private void CreateScript(string name, string path, string solution, string projectName)
        {
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }

            var cpp = Path.GetFullPath(Path.Combine(path, $"{name}.cpp"));
            using (var sw = File.CreateText(cpp))
            {
                sw.Write(string.Format(_cppCode, name, _namespace));
            }

            var h = Path.GetFullPath(Path.Combine(path, $"{name}.h"));
            using (var sw = File.CreateText(h))
            {
                sw.Write(string.Format(_hCode, name, _namespace));
            }

            string[] files = new string[] { cpp, h };
            for (int i = 0; i < 3; ++i)
            {
                if (!VisualStudio.AddFilesToSolution(solution, projectName, files))
                {
                    Thread.Sleep(1000);
                }
                else
                {
                    break;
                }
            }
        }

        private static string GetNamespaceFromProjectName()
        {
            var projectName = Project.CurrentGameProject!.Name;
            projectName = projectName.Replace(' ', '_');
            return projectName;
        }

        private bool Validate()
        {
            bool isValid = false;
            var name = scriptName.Text.Trim();
            var path = scriptPath.Text.Trim();

            var currentProjectPath = Project.CurrentGameProject!.Path;
            string errorMsg = string.Empty;

            if (string.IsNullOrEmpty(name))
            {
                errorMsg = "Type in a script name.";
            }
            else if (name.IndexOfAny(Path.GetInvalidFileNameChars()) != -1 || name.Any(x => char.IsWhiteSpace(x)))
            {
                errorMsg = "Invalid character(s) used in script name.";
            }
            else if (string.IsNullOrEmpty(path))
            {
                errorMsg = "Select a valid script folder";
            }
            else if (path.IndexOfAny(Path.GetInvalidPathChars()) != -1)
            {
                errorMsg = "Invalid character(s) used in script path.";
            }
            else if (!Path.GetFullPath(Path.Combine(currentProjectPath, path)).Contains(Path.Combine(currentProjectPath, @"GameCode\")))
            {
                errorMsg = "Script must be added to a (sub-folder of) GameCode.";
            }
            else if (File.Exists(Path.GetFullPath(Path.Combine(Path.Combine(currentProjectPath, path), $"{name}.cpp"))) ||
                     File.Exists(Path.GetFullPath(Path.Combine(Path.Combine(currentProjectPath, path), $"{name}.h"))))
            {
                errorMsg = $"Script {name} already exists in this folder.";
            }
            else
            {
                isValid = true;
            }

            if (!isValid)
            {
                messageTextBlock.Foreground = FindResource("Editor.RedBrush") as Brush;
            }
            else
            {
                messageTextBlock.Foreground = FindResource("Editor.FontBrush") as Brush;
            }

            messageTextBlock.Text = errorMsg;
            return isValid;
        }
    }
}
