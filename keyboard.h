/**********************************************************
 * Qt6 Virtual Keyboard Widget with Chinese Input Support
 * 支持拼音输入中文
 **********************************************************/

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QMap>
#include <QKeyEvent>

// 中文候选词显示控件
class ChineseWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ChineseWidget(QWidget *parent = nullptr);

    // 设置拼音，显示候选汉字
    void setPinyin(const QString &pinyin);

    // 清空候选
    void clear();

signals:
    void candidateSelected(const QString &text);

private slots:
    void onItemClicked(QListWidgetItem *item);

private:
    void addCandidate(const QString &text);
    void loadPinyinDict();  // 加载拼音词典

private:
    QMap<QString, QStringList> m_pinyinDict;  // 拼音->汉字映射
};

// 键盘按钮
class KeyboardButton : public QPushButton
{
    Q_OBJECT
public:
    explicit KeyboardButton(const QString &text, int keyCode, QWidget *parent = nullptr);

    void setNormalImage(const QString &imagePath);
    void setPressedImage(const QString &imagePath);
    void setKeyCode(int code) { m_keyCode = code; }
    int keyCode() const { return m_keyCode; }

    // 设置显示文本（用于大小写切换）
    void setDisplayText(const QString &text);

signals:
    void keyPressed(int keyCode, const QString &text);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    int m_keyCode;
    QString m_normalImage;
    QString m_pressedImage;

    void updateStyleSheet();
};

class Keyboard : public QWidget
{
    Q_OBJECT
public:
    enum KeyboardMode {
        LowerCase,    // 小写字母
        UpperCase,    // 大写字母
        Number        // 数字符号
    };

    enum InputMode {
        English,      // 英文输入
        Chinese       // 中文输入
    };

    explicit Keyboard(QWidget *parent = nullptr);

    // 设置目标输入控件
    void setTargetWidget(QWidget *widget);

    // 样式设置
    void setButtonStyleSheet(const QString &styleSheet);
    void setButtonSize(const QSize &size);
    void setButtonBackgroundImage(const QString &normalImage, const QString &pressedImage = QString());

    // 键盘模式
    void setKeyboardMode(KeyboardMode mode);
    KeyboardMode currentKeyboardMode() const { return m_keyboardMode; }

    // 输入模式（中/英文）
    void setInputMode(InputMode mode);
    InputMode currentInputMode() const { return m_inputMode; }

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void keyClicked(int keyCode, const QString &text);

private slots:
    void onKeyButtonPressed(int keyCode, const QString &text);
    void onCapsLockToggled();
    void onModeChanged();
    void onInputModeChanged();
    void onBackspacePressed();
    void onEnterPressed();
    void onCandidateSelected(const QString &text);

private:
    void setupUI();
    void createLetterKeyboard();
    void createNumberKeyboard();
    void updateKeyboardDisplay();

    KeyboardButton* createButton(const QString &text, int keyCode);
    void addButtonToLayout(QGridLayout *layout, const QString &text, int keyCode,
                           int row, int col, int rowSpan = 1, int colSpan = 1);

    void sendKeyEventToTarget(int keyCode, const QString &text);
    void sendTextToTarget(const QString &text);

private:
    // 布局常量
    static constexpr double KEYBOARD_ASPECT_RATIO = 2.5;  // 键盘宽高比
    static constexpr int KEYBOARD_MARGIN = 20;             // 键盘边距
    static constexpr int HALF_BUTTON_WIDTH = 30;           // 半个按钮宽度（用于错位布局）
    static constexpr int FULL_BUTTON_WIDTH = 60;           // 一个按钮宽度（用于错位布局）

private:
    QWidget *m_targetWidget;
    QVBoxLayout *m_mainLayout;

    // 中文候选词控件
    ChineseWidget *m_chineseWidget;

    // 字母键盘和数字键盘
    QWidget *m_letterWidget;
    QWidget *m_numberWidget;
    QGridLayout *m_letterLayout;
    QGridLayout *m_numberLayout;

    // 模式状态
    KeyboardMode m_keyboardMode;  // 键盘模式（大小写/数字）
    InputMode m_inputMode;        // 输入模式（中英文）
    bool m_capsLock;

    // 中文输入缓冲
    QString m_pinyinBuffer;

    // 按钮引用（用于更新显示）
    QMap<QString, KeyboardButton*> m_letterButtons;
    KeyboardButton *m_inputModeButton;  // 中英文切换按钮

    // 样式配置
    QSize m_buttonSize;
    QString m_buttonStyleSheet;
    QString m_normalImage;
    QString m_pressedImage;
};

#endif // KEYBOARD_H
