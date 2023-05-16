#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include "MainWindow.h"
#include "filesystem.h"

MainWindow::MainWindow() : QDialog()
{

    setWindowTitle("floats to hdr");

    resize(300, 400);

    auto pMainWindowLayout = new QGridLayout(this);

    m_pFileButton = new QPushButton("Browse...",this);
    pMainWindowLayout->addWidget(m_pFileButton, 0, 0, 1, 4);

    m_pFileEdit = new QTextEdit(this);
    pMainWindowLayout->addWidget(m_pFileEdit, 1, 0, 3, 0);

    auto pWidthLabel = new QLabel("Width:",this);
    pMainWindowLayout->addWidget(pWidthLabel, 4, 0);

    m_pImageWidthSpinbox = new QSpinBox(this);
    m_pImageWidthSpinbox->setMinimum(0);
    m_pImageWidthSpinbox->setMaximum(99990);
    pMainWindowLayout->addWidget(m_pImageWidthSpinbox, 4, 1);

    auto pHeightLabel = new QLabel("Height:",this);
    pMainWindowLayout->addWidget(pHeightLabel, 4, 2);

    m_pImageHeightSpinbox = new QSpinBox(this);
    m_pImageHeightSpinbox->setMinimum(0);
    m_pImageHeightSpinbox->setMaximum(99990);
    pMainWindowLayout->addWidget(m_pImageHeightSpinbox, 4, 3);

    auto pRotationLabel = new QLabel("Rotate by:", this);
    pMainWindowLayout->addWidget(pRotationLabel, 5, 0, 1, 2);

    m_pImageRotateSpinbox = new QComboBox(this);
    m_pImageRotateSpinbox->addItem("0°deg", 0);
    m_pImageRotateSpinbox->addItem("90°deg", 1);
    m_pImageRotateSpinbox->addItem("180°deg", 2);
    m_pImageRotateSpinbox->addItem("270°deg", 3);

    pMainWindowLayout->addWidget(m_pImageRotateSpinbox, 5, 2, 1, 2);

    m_pSaveFileButton = new QPushButton("Save as...",this);
    pMainWindowLayout->addWidget(m_pSaveFileButton, 6, 0, 1, 4);

    connect(m_pFileButton, &QPushButton::pressed, this, [&]{
        auto filePath = QFileDialog::getOpenFileName(this, "Open Float File","./","Float file (*.txt)" );

        if(filePath.isEmpty())
            return;

        QFile file(filePath);
        file.open(QFile::ReadOnly);

        QString data;
        data.append(file.readAll());
        data.replace(",", ".");
        auto list = data.split('\n');
        for(int i = 0; i < list.count() ; i++)
        {
           bool isOk;
            list[i].toFloat(&isOk);
            if(!isOk) {
                QMessageBox::critical(this, "Invalid Float File", "The file you're trying to process does not contain only floats.");
                return;
            }
        }

        m_pFileEdit->setText(data);

    });

    connect(m_pSaveFileButton, &QPushButton::pressed, this, [&]{


        auto list = m_pFileEdit->toPlainText().split('\n');

        uint width = m_pImageWidthSpinbox->value();
        uint height = m_pImageHeightSpinbox->value();

        if(width <= 0 || height <= 0 || width * height > list.count())
        {
            QMessageBox::critical(this, "Invalid Size", "The width and height specified are invalid.");
            return;
        }

        auto redData = QVector<float>(width * height);
        auto restData = QVector<float>(width * height);

        for(int i = 0; i < list.count() ; i++)
        {
            bool isOk;
            redData[i] = list[i].toFloat(&isOk);
            restData[i] = 0;
            if(!isOk) {
                QMessageBox::critical(this, "Invalid Float File", "The file you're trying to process does not contain only floats.");
                return;
            }
        }


        auto handle = CFIleSystem::createHandle(CFIleSystem::RLE_RGBE, m_pImageWidthSpinbox->value(), m_pImageHeightSpinbox->value(), redData.data(), restData.data(), restData.data() );
        for(int i = 0; i < m_pImageRotateSpinbox->currentData().toInt(); i++)
        {
            CFIleSystem::rotate90Degrees(handle);
        }

        CFIleSystem::writeHDR("/home/trico/Documents/GithubProjects/32fp2hdr/256floats.hdr", handle);


    });


}
