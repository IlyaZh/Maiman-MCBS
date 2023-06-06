function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut",
                                    "@TargetDir@/MaimanMultibench.exe",// target
                                    "@DesktopDir@/Maiman Multibench.lnk",// link-path
                                    "workingDirectory=@TargetDir@",// working-dir
                                    "iconPath=@TargetDir@/MaimanMultibench.exe", "iconId=0",// icon
                                    "description=Start MaimanMultibench");
    }
}
