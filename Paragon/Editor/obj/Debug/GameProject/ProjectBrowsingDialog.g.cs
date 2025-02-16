﻿#pragma checksum "..\..\..\GameProject\ProjectBrowsingDialog.xaml" "{ff1816ec-aa5e-4d10-87f7-6f4963833460}" "4E672975A25DCAAF12C981F3867C26ECB5650963"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.42000
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using Editor.GameProject;
using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Controls.Ribbon;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Shell;


namespace Editor.GameProject {
    
    
    /// <summary>
    /// ProjectBrowsingDialog
    /// </summary>
    public partial class ProjectBrowsingDialog : System.Windows.Window, System.Windows.Markup.IComponentConnector {
        
        
        #line 63 "..\..\..\GameProject\ProjectBrowsingDialog.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Canvas canvas;
        
        #line default
        #line hidden
        
        
        #line 65 "..\..\..\GameProject\ProjectBrowsingDialog.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Shapes.Rectangle highlightRect;
        
        #line default
        #line hidden
        
        
        #line 112 "..\..\..\GameProject\ProjectBrowsingDialog.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Primitives.ToggleButton openProjectButton;
        
        #line default
        #line hidden
        
        
        #line 119 "..\..\..\GameProject\ProjectBrowsingDialog.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Primitives.ToggleButton createProjectButton;
        
        #line default
        #line hidden
        
        
        #line 128 "..\..\..\GameProject\ProjectBrowsingDialog.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.StackPanel browserContent;
        
        #line default
        #line hidden
        
        
        #line 137 "..\..\..\GameProject\ProjectBrowsingDialog.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal Editor.GameProject.OpenProjectView openProjectView;
        
        #line default
        #line hidden
        
        
        #line 138 "..\..\..\GameProject\ProjectBrowsingDialog.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal Editor.GameProject.CreateProjectView createProjectView;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "8.0.0.0")]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/Editor;component/gameproject/projectbrowsingdialog.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\GameProject\ProjectBrowsingDialog.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "8.0.0.0")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1811:AvoidUncalledPrivateCode")]
        internal System.Delegate _CreateDelegate(System.Type delegateType, string handler) {
            return System.Delegate.CreateDelegate(delegateType, this, handler);
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "8.0.0.0")]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Maintainability", "CA1502:AvoidExcessiveComplexity")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1800:DoNotCastUnnecessarily")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            this.canvas = ((System.Windows.Controls.Canvas)(target));
            return;
            case 2:
            this.highlightRect = ((System.Windows.Shapes.Rectangle)(target));
            return;
            case 3:
            this.openProjectButton = ((System.Windows.Controls.Primitives.ToggleButton)(target));
            
            #line 114 "..\..\..\GameProject\ProjectBrowsingDialog.xaml"
            this.openProjectButton.Click += new System.Windows.RoutedEventHandler(this.OnOpenProjectButton_Click);
            
            #line default
            #line hidden
            return;
            case 4:
            this.createProjectButton = ((System.Windows.Controls.Primitives.ToggleButton)(target));
            
            #line 121 "..\..\..\GameProject\ProjectBrowsingDialog.xaml"
            this.createProjectButton.Click += new System.Windows.RoutedEventHandler(this.OnCreateProjectButton_Click);
            
            #line default
            #line hidden
            return;
            case 5:
            this.browserContent = ((System.Windows.Controls.StackPanel)(target));
            return;
            case 6:
            this.openProjectView = ((Editor.GameProject.OpenProjectView)(target));
            return;
            case 7:
            this.createProjectView = ((Editor.GameProject.CreateProjectView)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}

