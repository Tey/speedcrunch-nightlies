// This file is part of the SpeedCrunch project
// Copyright (C) 2004 Ariya Hidayat <ariya@kde.org>
// Copyright (C) 2005, 2006 Johan Thelin <e8johan@gmail.com>
// Copyright (C) 2007-2016 @heldercorreia
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.

#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include "gui/keypad.h"
#include "math/quantity.h"

#include <QMainWindow>

class AutoHideLabel;
class BitFieldWidget;
template <class Widget> class GenericDock;
class BookDock;
class Constants;
class ConstantsWidget;
class Editor;
class Evaluator;
class HistoryEntry;
class UserFunction;
class FunctionRepo;
class FunctionsWidget;
class HistoryWidget;
class ManualWindow;
class ManualServer;
class ResultDisplay;
class Session;
class Settings;
class UserFunctionListWidget;
class Variable;
class VariableListWidget;

class QActionGroup;
class QHBoxLayout;
class QLabel;
class QPlainTextEdit;
class QPushButton;
class QTranslator;
class QVBoxLayout;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

signals:
    void angleUnitChanged();
    void colorSchemeChanged();
    void languageChanged();
    void radixCharacterChanged();
    void resultFormatChanged();
    void resultPrecisionChanged();
    void syntaxHighlightingChanged();
    void historyChanged();
    void variablesChanged();
    void functionsChanged();

public slots:
    void copy();

private slots:
    void applyColorSchemeFromAction(QAction*);
    void applySelectedColorScheme();
    void clearEditor();
    void clearEditorAndBitfield();
    void clearHistory();
    void copyResultToClipboard();
    void cycleAngleUnits();
    void cycleResultFormats();
    void decreaseDisplayFontPointSize();
    void decreaseOpacity();
    void deleteVariables();
    void deleteUserFunctions();
    void evaluateEditorExpression();
    void exportHtml();
    void exportPlainText();
    void handleAutoCalcMessageAvailable(const QString&);
    void handleAutoCalcQuantityAvailable(const Quantity&);
    void handleCopyAvailable(bool copyAvailable);
    void handleBitsChanged(const QString&);
    void handleKeypadButtonPress(Keypad::Button);
    void handleEditorTextChange();
    void handleDisplaySelectionChange();
    void handleEditorSelectionChange();
    void handleManualClosed();
    void handleDockWidgetVisibilityChanged(bool visible);
    void hideStateLabel();
    void increaseDisplayFontPointSize();
    void increaseOpacity();
    void insertConstantIntoEditor(const QString&);
    void insertFunctionIntoEditor(const QString&);
    void insertTextIntoEditor(const QString&);
    void insertVariableIntoEditor(const QString&);
    void insertUserFunctionIntoEditor(const QString&);
    void openUpdatesURL();
    void openFeedbackURL();
    void openCommunityURL();
    void openNewsURL();
    void openDonateURL();
    void retranslateText();
    void revertColorScheme();
    void saveColorSchemeToRevert();
    void saveSessionDialog();
    void selectEditorExpression();
    void setAlwaysOnTopEnabled(bool);
    void setAngleModeDegree();
    void setAngleModeRadian();
    void setAutoAnsEnabled(bool);
    void setAutoCalcEnabled(bool);
    void setAutoCompletionEnabled(bool);
    void setBitfieldVisible(bool);
    void setConstantsDockVisible(bool, bool takeFocus = true);
    void setFormulaBookDockVisible(bool, bool takeFocus = true);
    void setFullScreenEnabled(bool);
    void setFunctionsDockVisible(bool, bool takeFocus = true);
    void setHistoryDockVisible(bool, bool takeFocus = true);
    void setSessionSaveEnabled(bool);
    void setKeypadVisible(bool);
    void setLeaveLastExpressionEnabled(bool);
    void setRadixCharacterAutomatic();
    void setRadixCharacter(char);
    void setRadixCharacterComma();
    void setRadixCharacterDot();
    void setRadixCharacterBoth();
    void setResultFormatBinary();
    void setResultFormatCartesian();
    void setResultFormat(char);
    void setResultFormatEngineering();
    void setResultFormatFixed();
    void setResultFormatGeneral();
    void setResultFormatHexadecimal();
    void setResultFormatOctal();
    void setResultFormatPolar();
    void setResultFormatScientific();
    void setResultPrecision15Digits();
    void setResultPrecision2Digits();
    void setResultPrecision3Digits();
    void setResultPrecision50Digits();
    void setResultPrecision8Digits();
    void setResultPrecisionAutomatic();
    void setResultPrecision(int);
    void setStatusBarVisible(bool);
    void setSyntaxHighlightingEnabled(bool);
    void setDigitGrouping(QAction*);
    void setAutoResultToClipboardEnabled(bool);
    void setComplexNumbers(bool);
    void setVariablesDockVisible(bool, bool takeFocus = true);
    void setUserFunctionsDockVisible(bool, bool takeFocus = true);
    void setWindowPositionSaveEnabled(bool);
    void setWidgetsDirection();
    void showAboutDialog();
    void showStateLabel(const QString&);
    void showFontDialog();
    void showLanguageChooserDialog();
    void showManualWindow();
    void showContextHelp();
    void showReadyMessage();
    void showResultFormatContextMenu(const QPoint&);
    void showSessionImportDialog();
    void showSessionLoadDialog();
    void wrapSelection();

protected:
    virtual void closeEvent(QCloseEvent*);
    virtual bool eventFilter(QObject*, QEvent*);

private:
    Q_DISABLE_COPY(MainWindow)

    void clearTextEditSelection(QPlainTextEdit*);
    void addTabifiedDock(QDockWidget*, bool takeFocus, Qt::DockWidgetArea = Qt::RightDockWidgetArea);
    void deleteDock(QDockWidget*);
    void createUi();
    void createActions();
    void createActionGroups();
    void createActionShortcuts();
    void createMenus();
    void createStatusBar();
    void createFixedWidgets();
    void createKeypad();
    void createBitField();
    void createBookDock(bool takeFocus = true);
    void createConstantsDock(bool takeFocus = true);
    void createFunctionsDock(bool takeFocus = true);
    void createHistoryDock(bool takeFocus = true);
    void createVariablesDock(bool takeFocus = true);
    void createUserFunctionsDock(bool takeFocus = true);
    void createFixedConnections();
    void applySettings();
    void checkInitialResultFormat();
    void checkInitialComplexFormat();
    void checkInitialResultPrecision();
    void checkInitialLanguage();
    void checkInitialDigitGrouping();
    void restoreSession();
    void deleteKeypad();
    void deleteStatusBar();
    void deleteBitField();
    void deleteBookDock();
    void deleteConstantsDock();
    void deleteFunctionsDock();
    void deleteHistoryDock();
    void deleteVariablesDock();
    void deleteUserFunctionsDock();
    void saveSettings();
    void saveSession(QString &fname);
    void setActionsText();
    void setMenusText();
    void setStatusBarText();

    static QTranslator* createTranslator(const QString& langCode);

    struct {
        QAction* sessionLoad;
        QAction* sessionSave;
        QAction* sessionImport;
        QAction* sessionExportHtml;
        QAction* sessionExportPlainText;
        QAction* sessionQuit;
        QAction* editCopy;
        QAction* editCopyLastResult;
        QAction* editPaste;
        QAction* editSelectExpression;
        QAction* editInsertFunction;
        QAction* editInsertVariable;
        QAction* editDeleteVariable;
        QAction* editInsertUserFunction;
        QAction* editDeleteUserFunction;
        QAction* editClearExpression;
        QAction* editClearHistory;
        QAction* editWrapSelection;
        QAction* viewKeypad;
        QAction* viewFormulaBook;
        QAction* viewConstants;
        QAction* viewFunctions;
        QAction* viewVariables;
        QAction* viewUserFunctions;
        QAction* viewHistory;
        QAction* viewStatusBar;
        QAction* viewFullScreenMode;
        QAction* viewBitfield;
        QAction* settingsResultFormatGeneral;
        QAction* settingsResultFormatFixed;
        QAction* settingsResultFormatEngineering;
        QAction* settingsResultFormatScientific;
        QAction* settingsResultFormatAutoPrecision;
        QAction* settingsResultFormat0Digits;
        QAction* settingsResultFormat2Digits;
        QAction* settingsResultFormat3Digits;
        QAction* settingsResultFormat8Digits;
        QAction* settingsResultFormat15Digits;
        QAction* settingsResultFormat50Digits;
        QAction* settingsResultFormatBinary;
        QAction* settingsResultFormatOctal;
        QAction* settingsResultFormatCartesian;
        QAction* settingsResultFormatPolar;
        QAction* settingsResultFormatHexadecimal;
        QAction* settingsAngleUnitRadian;
        QAction* settingsAngleUnitDegree;
        QAction* settingsBehaviorSaveSessionOnExit;
        QAction* settingsBehaviorSaveWindowPositionOnExit;
        QAction* settingsBehaviorPartialResults;
        QAction* settingsBehaviorAutoCompletion;
        QAction* settingsBehaviorSyntaxHighlighting;
        QAction* settingsBehaviorDigitGroupingNone;
        QAction* settingsBehaviorDigitGroupingOneSpace;
        QAction* settingsBehaviorDigitGroupingTwoSpaces;
        QAction* settingsBehaviorDigitGroupingThreeSpaces;
        QAction* settingsBehaviorAutoAns;
        QAction* settingsBehaviorLeaveLastExpression;
        QAction* settingsBehaviorAlwaysOnTop;
        QAction* settingsBehaviorAutoResultToClipboard;
        QAction* settingsRadixCharBoth;
        QAction* settingsBehaviorComplexNumbers;
        QAction* settingsDisplayZoomIn;
        QAction* settingsDisplayZoomOut;
        QAction* settingsDisplayFont;
        QVector<QAction*> settingsDisplayColorSchemes;
        QAction* settingsRadixCharDefault;
        QAction* settingsRadixCharDot;
        QAction* settingsRadixCharComma;
        QAction* settingsLanguage;
        QAction* helpManual;
        QAction* helpUpdates;
        QAction* helpFeedback;
        QAction* helpCommunity;
        QAction* helpNews;
        QAction* helpDonate;
        QAction* helpAbout;
        QAction* contextHelp;
    } m_actions;

    struct {
        QActionGroup* angle;
        QActionGroup* colorScheme;
        QActionGroup* digits;
        QActionGroup* resultFormat;
        QActionGroup* complexFormat;
        QActionGroup* radixChar;
        QActionGroup* digitGrouping;
    } m_actionGroups;

    struct {
        QMenu* angleUnit;
        QMenu* behavior;
        QMenu* colorScheme;
        QMenu* decimal;
        QMenu* digitGrouping;
        QMenu* display;
        QMenu* edit;
        QMenu* resultFormat;
        QMenu* inputFormat;
        QMenu* complexFormat;
        QMenu* help;
        QMenu* precision;
        QMenu* radixChar;
        QMenu* session;
        QMenu* sessionExport;
        QMenu* settings;
        QMenu* view;
    } m_menus;

    struct {
        QVBoxLayout* root;
        QHBoxLayout* keypad;
    } m_layouts;

    struct {
        QLabel* state;
        ResultDisplay* display;
        Editor* editor;
        Keypad* keypad = nullptr;
        QWidget* root;
        ManualWindow* manual = nullptr;
        BitFieldWidget* bitField = nullptr;
    } m_widgets;

    struct {
        BookDock* book;
        GenericDock<ConstantsWidget>* constants;
        GenericDock<FunctionsWidget>* functions;
        GenericDock<HistoryWidget>* history;
        GenericDock<VariableListWidget>* variables;
        GenericDock<UserFunctionListWidget>* userFunctions;
    } m_docks;
    QList<QDockWidget*> m_allDocks;

    struct {
        bool autoAns;
    } m_conditions;

    struct {
        QPushButton* angleUnit;
        QPushButton* resultFormat;
    } m_status;

    Constants* m_constants;
    Evaluator* m_evaluator;
    FunctionRepo* m_functions;
    Settings* m_settings;
    Session* m_session;
    QTranslator* m_translator;
    QPlainTextEdit* m_copyWidget;
    ManualServer* m_manualServer;
    QString m_colorSchemeToRevert;
};

#endif // GUI_MAINWINDOW_H
