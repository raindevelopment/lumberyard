/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#include <gtest/gtest.h>

#include <QAction>
#include <QtTest>
#include <QThread>
#include <qtconcurrent/qtconcurrentrun.h>


#include <Tests/UI/UIFixture.h>
#include <EMotionStudio/EMStudioSDK/Source/EMStudioManager.h>
#include <EMotionFX/CommandSystem/Source/CommandManager.h>
#include <EMotionFX/Source/AnimGraphManager.h>
#include <EMotionFX/Source/MotionManager.h>
#include <EMotionStudio/EMStudioSDK/Source/EMStudioManager.h>
#include <EMotionStudio/EMStudioSDK/Source/ResetSettingsDialog.h>
#include <EMotionStudio/EMStudioSDK/Source/SaveChangedFilesManager.h>

#include <EMotionFX/Source/Actor.h>
#include <EMotionFX/Source/AutoRegisteredActor.h>
#include <Tests/TestAssetCode/SimpleActors.h>
#include <Tests/TestAssetCode/ActorFactory.h>


namespace EMotionFX
{

    TEST_F(UIFixture, CanResetFromFileMenu)
    {
        /*
        Test Case: C16302179:
        Can reset from file menu.
        Fills a blank workspace with: an Actor (and instance); AnimGraph; MotionSet and Motion, then resets the workspace via the file menu -> reset.
        The "Reset" action blocks control flow waiting for user input, so a second thread is opened to click the "Discard Changes" button.
        */

        RecordProperty("test_case_id", "C16302179");

        const AZStd::string motionAsset("Gems/EMotionFX/Code/Tests/TestAssets/Rin/rin_idle.motion");
        const AZStd::string createAnimGraphCmd("CreateAnimGraph");
        const AZStd::string motionSetName("TestMotionSet");
        const AZStd::string createMotionSetCmd("CreateMotionSet -motionSetID 42 -name " + motionSetName);
        const AZStd::string createMotionCmd("ImportMotion -filename " + motionAsset);

        // Verify initial conditions
        ASSERT_EQ(GetActorManager().GetNumActors(), 0) << "Expected to see no actors yet";
        ASSERT_EQ(GetActorManager().GetNumActorInstances(), 0) << "Expected to see no actor instances";
        ASSERT_EQ(GetAnimGraphManager().GetNumAnimGraphs(), 0) << "Anim graph manager should contain 0 anim graphs.";
        ASSERT_EQ(GetMotionManager().GetNumMotionSets(), 0) << "Expected exactly zero motion sets";
        ASSERT_EQ(GetMotionManager().GetNumMotions(), 0) << "Expected exactly zero motions";

        // Create Actor, AnimGraph, Motionset and Motion
        AutoRegisteredActor actor = ActorFactory::CreateAndInit<SimpleJointChainActor>(2, "SampleActor");
        ActorInstance* actorInstance = ActorInstance::Create(actor.get());
        {
            AZStd::string result;
            ASSERT_TRUE(CommandSystem::GetCommandManager()->ExecuteCommand(createAnimGraphCmd, result)) << result.c_str();
            ASSERT_TRUE(CommandSystem::GetCommandManager()->ExecuteCommand(createMotionSetCmd, result)) << result.c_str();
            ASSERT_TRUE(CommandSystem::GetCommandManager()->ExecuteCommand(createMotionCmd, result)) << result.c_str();
        }

        // Verify pre-conditions
        ASSERT_EQ(GetActorManager().GetNumActors(), 1) << "Expected to see one actor";
        ASSERT_EQ(GetActorManager().GetNumActorInstances(), 1) << "Expected to see one actor instance";
        ASSERT_EQ(GetAnimGraphManager().GetNumAnimGraphs(), 1) << "Anim graph manager should contain 1 anim graph.";
        ASSERT_EQ(EMotionFX::GetMotionManager().GetNumMotionSets(), 1) << "Expected exactly one motion set";
        ASSERT_EQ(GetMotionManager().GetNumMotions(), 1) << "Expected exactly one motion";

        // Trigger reset from menu
        QAction* reset = GetNamedAction(EMStudio::GetMainWindow(), "&Reset");
        ASSERT_TRUE(reset) << "Could not find 'reset' action";

        const QString widgetToClick = "EMFX.SaveDirtySettingsWindow.DiscardButton";

        // Lambda for clicking the "discard" button on a modal dialog
        auto onFocusedChanged = [widgetToClick](QWidget* old, QWidget* now)
        {
            int stall = 0;
            QDialog* dialog = qobject_cast<QDialog*>(now);
            if (dialog)
            {
                QWidget* widget = UIFixture::FindTopLevelWidget(widgetToClick);
                if (widget)
                {
                    QTest::mouseClick(widget, Qt::LeftButton, Qt::NoModifier, widget->rect().center());
                }
            }
        };

        // Connect the lambda
        auto connection = QObject::connect(qApp, &QApplication::focusChanged, onFocusedChanged);

        // Click File -> Reset which will show the modal dialog box
        reset->trigger(); 

        // Disconnect listener
        QObject::disconnect(connection);

        // Find and accept Reset Settings Dialog box
        auto dialogBox = qobject_cast<EMStudio::ResetSettingsDialog*>(FindTopLevelWidget("EMFX.MainWindow.ResetSettingsDialog"));
        ASSERT_TRUE(dialogBox) << "Could not find ResetSettingsDialog widget";
        dialogBox->accept();

        // Ensure workspace was cleared
        EXPECT_EQ(GetActorManager().GetNumActors(), 0) << "Expected to see no actors";
        EXPECT_EQ(GetActorManager().GetNumActorInstances(), 0) << "Expected to see no actor instances";
        EXPECT_EQ(EMotionFX::GetAnimGraphManager().GetNumAnimGraphs(), 0) << "Anim graph manager should contain 0 anim graphs.";
        EXPECT_EQ(GetMotionManager().GetNumMotionSets(), 0) << "Expected exactly zero motion sets";
        EXPECT_EQ(GetMotionManager().GetNumMotions(), 0) << "Expected exactly zero motions";

        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    } // Test: CanResetFromFileMenu

} // namespace EMotionFX
