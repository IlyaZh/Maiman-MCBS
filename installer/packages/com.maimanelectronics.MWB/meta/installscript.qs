function Component()
{
    component.loaded.connect(this, addShortCutCheckBox);
}
addShortCutCheckBox = function()
{
    // don't show when updating or uninstalling
    if (installer.isInstaller()) {
        if (installer.addWizardPageItem(component, "ShortCutCheckBoxForm", QInstaller.TargetDirectory)) {
            component.userInterface("ShortCutCheckBoxForm")
        }
    }
}
Component.prototype.createOperations = function()
{
    try {
        component.createOperations();
        if (systemInfo.productType == "windows") {

            var checkboxForm = component.userInterface( "ShortCutCheckBoxForm" );
            if (checkboxForm.ShortCutCheckBox.checked) {
                component.addOperation("CreateShortcut",
                                            "@TargetDir@/MaimanMultibench.exe",// target
                                            "@DesktopDir@/Maiman Multibench.lnk",// link-path
                                            "workingDirectory=@TargetDir@",// working-dir
                                            "iconPath=@TargetDir@/MaimanMultibench.exe", "iconId=0",// icon
                                            "description=Start MaimanMultibench");
                }
            }

    } catch(e) {
        console.log(e);
    }
}
