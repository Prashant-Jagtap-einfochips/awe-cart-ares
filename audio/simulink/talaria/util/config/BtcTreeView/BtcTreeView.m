classdef BtcTreeView < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        AppFigure                  matlab.ui.Figure
        GridLayout                 matlab.ui.container.GridLayout
        LeftPanel                  matlab.ui.container.Panel
        Tree                       matlab.ui.container.Tree
        Node                       matlab.ui.container.TreeNode
        Node2                      matlab.ui.container.TreeNode
        RightPanel                 matlab.ui.container.Panel
        TextArea                   matlab.ui.control.TextArea
        TopPanel                   matlab.ui.container.Panel
        BtcVariableEditField       matlab.ui.control.EditField
        BtcVariableEditFieldLabel  matlab.ui.control.Label
        ValueEdit                  matlab.ui.control.UIControl
        SelectedVariableLabel      matlab.ui.control.Label
        Slider                     matlab.ui.control.Slider
        ResetButton                matlab.ui.control.Button
        PreValidateButton          matlab.ui.control.Button
        EditClassButton            matlab.ui.control.Button
        % Styles
        StyleBold                  matlab.ui.style.Style
        StyleDimText               matlab.ui.style.Style
        StyleIconLock              matlab.ui.style.Style
        StyleIconBtc               matlab.ui.style.Style
        StyleIconObj               matlab.ui.style.Style
        StyleIconProperty          matlab.ui.style.Style

        % Last key memory
        LastKey                    string
        LoadedEditString           string

        % The node styles make the whole tree look good and make sense.
        % However, the MathWorks implementation is SO SLOW largely due
        % to the arrayfun call made from
        % StyleableComponentController.getSerializableStyleConfigurationStorage
        % Looks like it's the presence of ICONS that's activating the
        % inefficient code. :-(
        UseStyles logical = 0
    end


    % Callbacks that handle component events
    methods (Access = private)
        function SliderValueChanging(app, event)
            changingValue = event.Value;
            leftWidth = sprintf('%dx', changingValue);
            rightWidth = sprintf('%dx', 100-changingValue);
            app.GridLayout.ColumnWidth = {leftWidth,rightWidth};
        end
        function ColumnPanelSizeChanged(~, event)
            newWidth = event.Source.Position(3);
            newHeight = event.Source.Position(4);
            event.Source.Children(1).Position = [1 1 newWidth newHeight];
        end
        function TopPanelSizeChanged(app,event)
            newWidth = event.Source.Position(3);
            app.Slider.Position = [1 1 newWidth 3];
            app.BtcVariableEditField.Position(3) = newWidth - app.BtcVariableEditField.Position(1) - 5;
            app.ValueEdit.Position(3) = newWidth - app.BtcVariableEditField.Position(1) - 5;
            editPos = app.BtcVariableEditField.Position;
            app.ResetButton.Position = [newWidth-45, editPos(2), 40, editPos(4)];
            app.PreValidateButton.Position = [newWidth-115, editPos(2), 70, editPos(4)];
            editPos = app.ValueEdit.Position;
            app.EditClassButton.Position = [editPos(1) editPos(2) 200 editPos(4)];
        end
        function TreeDoubleClick(app,event)
            theNode = event.InteractionInformation.Node;
            if (false == isempty(theNode.Children))
                theNode.Children.delete;
            end
            app.LoadObject(theNode.NodeData,theNode);
        end
        function TreeSelectionChange(app,event)
            app.UpdateTextArea(event.SelectedNodes(1));
        end
        function BtcVariableChanged(app,event)
            % Clear the entire tree
            app.Tree.Children.delete;
            % Load the new variable
            app.LoadObject(event.Value);
        end
        function OnValueEditKeyPress(app,event)
            app.LastKey = event.Key;
            switch (event.Key)
                case "escape"
                    app.Tree.focus;
                case "return"
                    % If the edit box has not changed value yet when the
                    % return, or enter, key is pressed, this callback gets
                    % invoked instead of the generic "callback" below
                    app.Tree.focus;
            end
        end
        function OnValueEditCallback(app,~)
            % Set the new value and loose focus
            app.HandleSelectedValueEditExit();
        end
        function ResetButtonPressed(app)
            theVarName = app.BtcVariableEditField.Value;
            try
                theVar = evalin('base',theVarName);
                theClass = class(theVar);
                evalin('base',sprintf('clear %s',theVarName));
                assignin('base',theVarName,feval(theClass));
                if (false == isempty(app.Tree.SelectedNodes))
                    app.UpdateTextArea(app.Tree.SelectedNodes(1));
                end
            catch ME
                fprintf('Unable to clear %s: [%s]\n',theVarName,ME.message);
            end
        end
        function PreValidateButtonPressed(app)
            theVarName = app.BtcVariableEditField.Value;
            theVar = evalin('base',theVarName);
            fprintf('---------- PreValidate of [%s] ----------\n',theVarName);
            BTConfig.PreValidate(theVar,theVarName);
        end
        function EditClassButtonPressed(app)
            theClass = app.EditClassButton.UserData;
            edit(theClass);
        end
    end

    % Component initialization
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)

            width = 800;
            height = 500;
            topHeight = 55;
            changingValue = 40;

            % Create BTCExploreUIFigure and hide until all components are created
            app.AppFigure = uifigure('Visible', 'off');
            app.AppFigure.AutoResizeChildren = 'off';
            app.AppFigure.Position = [100 100 width height];
            app.AppFigure.Name = 'BTC Tree View';

            % Create GridLayout
            app.GridLayout = uigridlayout(app.AppFigure);
            leftWidth = sprintf('%dx', changingValue);
            rightWidth = sprintf('%dx', 100-changingValue);            
            app.GridLayout.ColumnWidth = {leftWidth,rightWidth};
            app.GridLayout.RowHeight = {topHeight,'1x'};
            app.GridLayout.ColumnSpacing = 0;
            app.GridLayout.RowSpacing = 0;
            app.GridLayout.Padding = [0 0 0 0];
            app.GridLayout.Scrollable = 'on';

            % Create TopPanel
            app.TopPanel = uipanel(app.GridLayout);
            app.TopPanel.Layout.Row = 1;
            app.TopPanel.Layout.Column = [1 2];
            app.TopPanel.AutoResizeChildren = 'off';
            app.TopPanel.SizeChangedFcn = createCallbackFcn(app, @TopPanelSizeChanged, true);

            % Create LeftPanel
            app.LeftPanel = uipanel(app.GridLayout);
            app.LeftPanel.Layout.Row = 2;
            app.LeftPanel.Layout.Column = 1;
            app.LeftPanel.AutoResizeChildren = 'off';
            app.LeftPanel.SizeChangedFcn = createCallbackFcn(app, @ColumnPanelSizeChanged, true);

            % Create RightPanel
            app.RightPanel = uipanel(app.GridLayout);
            app.RightPanel.Layout.Row = 2;
            app.RightPanel.Layout.Column = 2;
            app.RightPanel.AutoResizeChildren = 'off';
            app.RightPanel.SizeChangedFcn = createCallbackFcn(app, @ColumnPanelSizeChanged, true);

            % Create Tree
            app.Tree = uitree(app.LeftPanel);
            app.Tree.Position = [1 1 255 399];
            app.Tree.FontName = 'Consolas';
            app.Tree.DoubleClickedFcn = createCallbackFcn(app,@TreeDoubleClick,true);
            app.Tree.SelectionChangedFcn = createCallbackFcn(app,@TreeSelectionChange,true);

            % Create TextArea
            app.TextArea = uitextarea(app.RightPanel);
            app.TextArea.Position = [1 1 255 399];
            app.TextArea.FontName = 'Consolas';
            app.TextArea.Editable = 'off';
 
            % Create Slider
            app.Slider = uislider(app.TopPanel);
            app.Slider.MajorTicks = [];
            app.Slider.ValueChangingFcn = createCallbackFcn(app, @SliderValueChanging, true);
            app.Slider.MinorTicks = [];
            app.Slider.Position = [1 1 499 3]; % [7 5 485 10]
            app.Slider.Value = changingValue;

            % Create BtcVariableEditFieldLabel
            app.BtcVariableEditFieldLabel = uilabel(app.TopPanel);
            app.BtcVariableEditFieldLabel.Position = [5 30 85 20];
            app.BtcVariableEditFieldLabel.Text = 'Root Variable:';

            % Create BtcVariableEditField
            app.BtcVariableEditField = uieditfield(app.TopPanel, 'text');
            app.BtcVariableEditField.Position = [90 30 410 20];
            app.BtcVariableEditField.ValueChangedFcn = createCallbackFcn(app,@BtcVariableChanged,true);

            % Create selected variable label
            app.SelectedVariableLabel = uilabel(app.TopPanel);
            app.SelectedVariableLabel.Position = [5 10 85 20];
            app.SelectedVariableLabel.Text = 'Selected Value:';
            
            % Created edit box for selected value. To support handling of
            % enter-key and escape-key use low-level uicontrol instead of
            % the supposedly handy textedit control
            app.ValueEdit = uicontrol(app.TopPanel, ...
                'Style','edit', ...
                'Position',[90 10 410 20], ...
                'HorizontalAlignment', 'left', ...
                'Callback',createCallbackFcn(app,@OnValueEditCallback,true),...
                'KeyPressFcn',createCallbackFcn(app,@OnValueEditKeyPress,true));

            app.ResetButton = uibutton(app.TopPanel);
            app.ResetButton.Text = 'Reset';
            app.ResetButton.ButtonPushedFcn = createCallbackFcn(app,@ResetButtonPressed);

            app.PreValidateButton = uibutton(app.TopPanel);
            app.PreValidateButton.Text = 'PreValidate';
            app.PreValidateButton.ButtonPushedFcn = createCallbackFcn(app,@PreValidateButtonPressed);

            app.EditClassButton = uibutton(app.TopPanel);
            app.EditClassButton.Text = 'Edit Class';
            app.EditClassButton.ButtonPushedFcn = createCallbackFcn(app,@EditClassButtonPressed);
            app.EditClassButton.Visible = "off";

            % Create Styles
            app.StyleBold     = uistyle("FontWeight","bold");
            app.StyleDimText  = uistyle("FontColor",[.5 .5 .5]);
            app.StyleIconLock     = uistyle("Icon","IconLock24.png"); 
            app.StyleIconBtc      = uistyle("Icon","IconBtc24.png");
            app.StyleIconObj      = uistyle("Icon","IconObj24.png");
            app.StyleIconProperty = uistyle("Icon","IconProp24.png"); 

            % Show the figure after all components are created
            app.AppFigure.Visible = 'on';
        end

    end

    methods (Access = private)
        function LoadObject(app,objectVarName,nodeObject)
            % LoadObject resets the tree to display the given variable,
            % which is pulled from the base workspace
            objectVarName = strtrim(objectVarName);
            if (false == exist('nodeObject','var'))
                nodeObject = uitreenode(app.Tree, ...
                    "Text",objectVarName, ...
                    "NodeData",objectVarName);
            end

            if (app.UseStyles)
                % Remove bold formatting of node text. This is hideously
                % inconvenient with the provided API, but achieveable
                % First acquire all styles for the entire tree
                allStyleRows = app.Tree.StyleConfigurations;
                % Select only those that match this node
                nodeStyleRows = allStyleRows(ismember([allStyleRows.TargetIndex{:}],nodeObject),:);
                % Iterate through matching styles looking for bold
                for s=1:size(nodeStyleRows,1)
                    if (isequal(nodeStyleRows.Style(s), app.StyleBold))
                        rowStr = nodeStyleRows.Row{s};
                        removeStyle(app.Tree,str2double(rowStr))
                        break; % one is enough
                    end
                end
            else
                % Clear off extra information now that the node is being
                % expanded
                currentNodeText = nodeObject.Text;
                currentNodeText = strrep(currentNodeText,' (Object)','');
                currentNodeText = strrep(currentNodeText,' (BTC)','');
                nodeObject.Text = currentNodeText;
            end

            % Acquire the actual object
            theObj = evalin('base',objectVarName);
            btcMetaClass = meta.class.fromName(class(theObj));
            if isa(theObj,'BtcLockAndDefaultClass')
                isBtcObject = true;
            else
                isBtcObject = false;
            end
            for i=1:numel(btcMetaClass.PropertyList)
                aProp = btcMetaClass.PropertyList(i);
                propPath = sprintf('%s.%s',objectVarName,aProp.Name);
                % Determine whether to list this property as a BTC element
                % for the btcObj
                [canGet,canSet,isBtcClassProp] = BtcBaseClass.GetPropertyStats(aProp);
                if (false == canGet)
                    % If we cannot even get it, give up now
                    continue;
                end
                if ((false == canSet) && (false == isBtcClassProp))
                    % If we cannot set it and it's not a user-defined class
                    % then also there's no reason to list it
                    continue;
                end
                % Get the property to list in correct style
                nodeText = sprintf('%s',aProp.Name);
                newNode = uitreenode(nodeObject, ...
                    "Text",nodeText, ...
                    "NodeData",propPath);

                % Apply styles to the tree node
                if (isBtcObject)
                    propIsLocked = theObj.IsLockedOrDefaulted(aProp.Name);
                else
                    propIsLocked = false;
                end
                % If "styles" are enabled, apply font and icon styles
                if (app.UseStyles)
                    if (isBtcClassProp)
                        addStyle(app.Tree,app.StyleBold,"node",newNode);
                        addStyle(app.Tree,app.StyleIconBtc,"node",newNode);
                    elseif (propIsLocked)
                        addStyle(app.Tree,app.StyleDimText,"node",newNode);
                        addStyle(app.Tree,app.StyleIconLock,"node",newNode);
                    else
                        addStyle(app.Tree,app.StyleIconProperty,"node",newNode);
                    end
                else
                    % Otherwise, with no styles, just use text
                    if (isBtcClassProp)
                        extraInfo = " (BTC)";
                    elseif (propIsLocked)
                        extraInfo = " (*Locked)";
                    else
                        extraInfo = "";
                    end
                    newNode.Text = sprintf('%s%s',nodeText,extraInfo);
                end
            end
            nodeObject.expand();
            app.UpdateTextArea(nodeObject);
        end
   
        function UpdateTextArea(app,theNode)
            thisName = theNode.Text;
            propertyPath = theNode.NodeData;
            text = sprintf("%s\n-------------------------",thisName);
            text(end+1) = sprintf("%s\n-------------------------",propertyPath);

            % Remove the edit interface until we know it can be used
            app.ValueEdit.Enable = 'off';

            try
                btcObj = evalin('base',propertyPath);
            catch
                % Failed to acquire the property path's value
                btcObj = [];
            end
            text(end+1) = formattedDisplayText(btcObj,'SuppressMarkup',true);

            % Handle enumerations
            if (isenum(btcObj))
                text(end+2) = 'Enumeration choices:';
                text(end+1) = formattedDisplayText(enumeration(btcObj));
                app.SetValueEditString(MaskUtil.ValueToStr(btcObj));
                app.ValueEdit.Enable = 'on';
                app.EditClassButton.Visible = "off";
            elseif (false == isobject(btcObj) || isstring(btcObj))
                app.SetValueEditString(MaskUtil.ValueToStr(btcObj));
                app.ValueEdit.Enable = 'on';
                app.EditClassButton.Visible = "off";
            else
                app.SetValueEditString('Object: editing not supported');
                app.EditClassButton.Visible = "on";
                app.EditClassButton.Text = sprintf('Edit %s',class(btcObj));
                app.EditClassButton.UserData = class(btcObj);
            end

            % Checks locks/defaults
            rootNode = app.Tree.Children(1);
            rootObject = evalin('base',rootNode.Text);
            if isa(rootObject,'BtcLockAndDefaultClass') && theNode ~= rootNode
                % Use the root object to expand the property path
                propPathForRoot = extractAfter(propertyPath,[rootNode.Text '.']);
                [objects,components] = rootObject.ExpandPropertyPath(propPathForRoot);
                propOwner = objects{end};
                propName = components(end);

                % Check if the property owner supports extra BTC goodies
                if (false == isa(propOwner,'BtcLockAndDefaultClass'))
                    text(end+1) = sprintf("\n***\nProperty owner class [%s]\n" + ...
                        "does not support Talaria BTC features.\n***", ...
                        class(propOwner));
                else
                    theEntry = propOwner.GetPropertyEntry(propName);
                    % Lay out the details of Locks and Defaults
                    if (false == isempty(theEntry))
                        if (theEntry.HasLock)
                            % Show Lock information
                            [theValue,isCalculated] = theEntry.GetLockValue();
                            text(end+1) = sprintf("\nLocked by [%s]",class(theEntry.LockControllingObject));
                            if (isCalculated)
                                text(end+1) = sprintf("Lock Expression: %s",formattedDisplayText(theEntry.LockValue,'SuppressMarkup',true));
                            end
                            text(end+1) = sprintf("Lock value: %s",formattedDisplayText(theValue,'SuppressMarkup',true));
                            % Disabled editing for locked properties
                            app.ValueEdit.Enable = 'off';
                        end
                        if (theEntry.HasDefault)
                            % Show Default information
                            [theValue,isCalculated] = theEntry.GetDefaultValue();
                            text(end+1) = sprintf("\nDefault set by [%s]",class(theEntry.DefaultControllingObject));
                            if (isCalculated)
                                text(end+1) = sprintf("Default Expression: %s",formattedDisplayText(theEntry.DefaultValue,'SuppressMarkup',true));
                            end
                            text(end+1) = sprintf("Default value: %s",formattedDisplayText(theValue,'SuppressMarkup',true));
                        end
    
                        % Characterize where the current value comes from:
                        [hasLock,usingDefault,lastSetBy] = propOwner.IsLockedOrDefaulted(propName);                    
                        if (hasLock && (false==usingDefault))
                            text(end+2) = 'Using locked value.';
                        elseif (hasLock && usingDefault)
                            text(end+2) = 'Using the default value because the locked value is unspecified';
                        elseif (usingDefault)
                            text(end+2) = 'Unspecified: Using the default value.';
                        else
                            text(end+2) = 'Using a specified value.';
                            cleanLastSet = regexprep(lastSetBy,'</?a[^>]*>','');
                            text(end+1) = sprintf('Value set by: %s',cleanLastSet);
                        end
                    end
                    % Also mention whether this property will export to tools based
                    % on validator definitions
                    [exportIt,~,exportControllingObject] = IsExportedToTools(propOwner,propName);
                    if (exportIt)
                        if (isempty(exportControllingObject))
                            exportedBy = ' (via property validator)';
                        else
                            exportedBy = sprintf(' (exported by %s)',class(exportControllingObject));
                        end
                        text(end+2) = ['Marked for ExportToTools' exportedBy];
                    end
                end
            end
            app.TextArea.Value = text;
        end

        function SetValueEditString(app,theString)
            % SetValueEditString sets value editbox string, and remembers
            % this string so any edits aborted with ESC key can easily
            % return to this "last set" value
            app.LoadedEditString = theString;
            app.ValueEdit.String = theString;
        end

        %------------------------------------------------------------------
        function HandleSelectedValueEditExit(app)
            % HandleSelectedValueEditExit takes action when the edit box
            % for the selected variable's value looses focus. Attempts to
            % handle the escape key to reset the edit box value failed - so
            % attempting to handle all that here by remembering the last 
            % key that was pressed.
            if (app.LastKey == "return")
                newValueAsText = app.ValueEdit.String;
                if (isempty(newValueAsText))
                    newValueAsText = '[]';
                end

                % Acquire the selected node's variable property path
                theNode = app.Tree.SelectedNodes(1);
                propertyPath = theNode.NodeData;

                % Attempt to set the variable
                setExpression = sprintf('%s = %s;', propertyPath,newValueAsText);
                try
                    % Interactively set a new value for the BTC as
                    % specified by the text edit box GUI control
                    evalin('base',setExpression);
                catch ME
                    error('Cannot change [%s] to [%s]:\n%s', ...
                        propertyPath,newValueAsText,ME.message);
                end

                % Update display with new value
                app.UpdateTextArea(theNode);
            else
                % Lost focus with something other than enter/return key.
                % Reset edit to last loaded value:
                app.SetValueEditString(app.LoadedEditString);
            end
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = BtcTreeView(rootObjectName,expandedPath)

            % Create UIFigure and components
            createComponents(app)

            % Finally, load up a default object
            initialVariableName = [];
            if (exist('rootObjectName','var'))
                if (false == ischar(rootObjectName) && false == isstring(rootObjectName))
                    error('Provided argument must be a variable name')
                end
                initialVariableName = rootObjectName;
            elseif (false == isempty(bdroot))
                [~,initialVariableName] = BTConfig.GetModelSettings();
            end
            if (false == isempty(initialVariableName))                
                app.BtcVariableEditField.Value = initialVariableName;
                app.LoadObject(app.BtcVariableEditField.Value);
                if (exist('expandedPath','var'))
                    app.ExpandPropertyPath(expandedPath)
                end
            end
            
            % Register the app with App Designer
            registerApp(app, app.AppFigure)

            if nargout == 0
                clear app
            end
        end

        % Code that executes before app deletion
        function delete(app)

            % Delete UIFigure when app is deleted
            delete(app.AppFigure)
        end

        % Attempt to expand a property path in the existing tree
        function ExpandPropertyPath(app,propertyPath)
            % ExpandPropertyPath attempts to expand the given path in tree
            % Usage:
            %    ExpandPropertyPath(app,propertyPath);
            splitComponents = strsplit(propertyPath,".");
            components = string(splitComponents);
            currentNode = app.Tree;
            % Loop through each piece of the path
            for i=1:numel(components)
                % Loop through tree child nodes of current node
                foundNode = false;
                if isempty(currentNode.Children)
                    % Not expanded yet - load it
                    app.LoadObject(currentNode.NodeData,currentNode);
                end
                for n=1:numel(currentNode.Children)
                    aNode = currentNode.Children(n);
                    % strip off extra information notes from node text
                    nodeText = regexp(aNode.Text,'\w*','match','once');
                    if (nodeText == components(i))
                        % found the node!
                        currentNode = aNode;
                        foundNode = true;
                        break; % on to next part of path
                    end
                end
                if (foundNode == false)
                    % Didn't find this one
                    if (i <= numel(components))
                        warning('Could not expand past %s', ...
                            strjoin(components(1:i-1),'.'));
                    end
                    break; % Done searching
                end
            end
            app.Tree.SelectedNodes = currentNode;
            app.UpdateTextArea(currentNode);
        end
    end
    methods(Static)
        function CreateOrUpdateTree(propertyPath)
            % CreateOrUpdateTree creates/updates tree, saving in base WS
            % Usage:
            %    BtcTreeView.CreateOrUpdateTree(propertyPath)

            % First break apart the path to find root var
            varNameParts = string(strsplit(propertyPath,'.'));
            rootVarName = varNameParts(1);
            % assemble the tree view variable name
            treeVarName = sprintf('%s_BtcTreeView',rootVarName);
            % either acquire the GUI variable, or make new one
            createNewView = false;
            try
                theTree = evalin('base',treeVarName);
                if (false == isvalid(theTree))
                    % object not valid. Create new one
                    createNewView = true;
                else
                    % Valid object found. Ensure it's still assocated with
                    % the root variable we want:
                    if (rootVarName ~= theTree.BtcVariableEditField.Value)
                        createNewView = true;
                    end
                end
            catch
                createNewView = true;
            end

            if (createNewView)
                assignin('base',treeVarName,BtcTreeView(rootVarName));
                theTree = evalin('base',treeVarName);
            end

            theTree.ExpandPropertyPath(propertyPath);

            % bring window to front
            figure(theTree.AppFigure)
        end
    end
end