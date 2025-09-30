#include "MainWindow.h"
#include "ui_MainWindow.h"
#include<QFileDialog>
#include<QUrl>
#include<QDir>
#include"HistoryDao.h"
#include"DBFactory.h"
#include"UserInfo.h"
#include"HistoryDialog.h"
#include<QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMedia();//初始化媒体播放
    QDateTime currentDateTime=QDateTime::currentDateTime();
    qint64 timestamp=currentDateTime.toSecsSinceEpoch();
    qsrand(timestamp);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_browser_clicked()
{
    //选择要播放文件的文件夹
    QString dirPath=QFileDialog::getExistingDirectory(this,"选择播放文件夹",".");
    //创建文件夹对象
    QDir dir(dirPath);
    //获得所有文件的名字
    QStringList sl=dir.entryList(QDir::Files);
    ui->listWidget_names->addItems(sl);//添加文件列表
    addToPlaylist(dirPath);
}
void MainWindow::initMedia()
{
    //创建对象
    player=new QMediaPlayer(this);//管理生命周期要有一个父节点
    playlist=new QMediaPlaylist(this);
    vWidget=new QVideoWidget(this);
    vWidget->setGeometry(0,0,400,400);
    //关联player和播放列表
    player->setPlaylist(playlist);
    //关联player和播放控件
    player->setVideoOutput(vWidget);
    vWidget->show();//显示控件
    connect(player,&QMediaPlayer::positionChanged,this,&MainWindow::onPositionChanged);
    connect(player,&QMediaPlayer::mediaStatusChanged,this,&MainWindow::onMediaStatusChanged);
}
void MainWindow::addToPlaylist(QString dir)
{
    QDir dirObj(dir);
    QStringList sl=dirObj.entryList(QDir::Files);
    for(auto file:sl)
    {
        QString filePath=dir+"/"+file;//构建完整路径
        playlist->addMedia(QUrl::fromLocalFile(filePath));//添加到播放列表
    }
}
void MainWindow::showTime()
{
    int totalTime=player->duration();//获得总时长
    int nowTime=player->position();//获得当前时间

    //总时间,将毫秒转换成总秒数
    int totalSeconds=totalTime / 1000;
    //分别计算小时、分、秒
    int thours=totalSeconds / 3600;
    int tminutes=(totalSeconds % 3600) / 60;
    int tseconds=totalSeconds % 60;

    //当前时间
    int nowSecond=nowTime / 1000;
    //分别计算时、分、秒
    int nhours=nowSecond / 3600;
    int nminutues=(nowSecond % 3600) / 60;
    int nsecond=nowSecond % 60;

    QString data=QString("%1:%2:%3/%4:%5:%6").arg(nhours).arg(nminutues).arg(nsecond).arg(thours).arg(tminutes).arg(tseconds);
    ui->label_time->setText(data);
}

void MainWindow::addToHistory()
{
    HistoryEntity he;
    he.userid=UserInfo::getInstance()->getUserid();
    he.name=ui->listWidget_names->currentItem()->text();
    DBFactory::getInstance()->getHistoryDao()->addHistory(he);
}

void MainWindow::nextMode()
{
    if(ui->radioButton_repeat->isChecked())//单曲循环按钮被选中
    {
        //继续使用当前的下标
        playlist->setCurrentIndex(playlist->currentIndex());
    }
    else if(ui->radioButton_random->isChecked())//随机
    {
        playlist->setCurrentIndex(qrand()%playlist->mediaCount());
    }
    else if(ui->radioButton_next->isChecked())//下一曲
    {
        on_pushButton_next_clicked();
    }
    ui->listWidget_names->setCurrentRow(playlist->currentIndex());
}
void MainWindow::on_listWidget_names_itemDoubleClicked(QListWidgetItem *item)
{
    //获得点击条目的索引
    int index=ui->listWidget_names->row(item);
    //设置播放列表为当前选中的媒体
    playlist->setCurrentIndex(index);
    player->play();//播放歌曲
    addToHistory();
    ui->pushButton_play->setIcon(QIcon(":/icon/pause.png"));
}

void MainWindow::on_pushButton_add_clicked()
{
    //获得网址
    QString urlStr=ui->lineEdit_url->text();
    //QlistWidget显示
    QStringList sl=urlStr.split('/');
    ui->listWidget_names->addItem(sl.last());//将文件名显示在列表中
    //添加到playlist
    playlist->addMedia(QUrl(urlStr));
}

void MainWindow::on_horizontalSlider_progess_sliderReleased()
{
    //设置当前播放进度
    player->setPosition(ui->horizontalSlider_progess->value());
}
//播放进度发生改变
void MainWindow::onPositionChanged(qint64 pos)
{
    showTime();//显示时间
    //设置进度条的位置
    ui->horizontalSlider_progess->setValue(pos);
}
void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    //加载完毕
    if(status==QMediaPlayer::BufferedMedia)
    {
        ui->horizontalSlider_progess->setMaximum(player->duration());
    }
    else if(status==QMediaPlayer::EndOfMedia)
    {
        nextMode();
    }
}

void MainWindow::on_pushButton_pre_clicked()
{
    int index=playlist->currentIndex()==0?
                playlist->mediaCount()-1:
                playlist->previousIndex();
    playlist->setCurrentIndex(index);
    ui->listWidget_names->setCurrentRow(playlist->currentIndex());
    player->play();
    addToHistory();
}

void MainWindow::on_pushButton_play_clicked()
{
    if(player->state()==QMediaPlayer::PlayingState)
    {
        player->pause();//暂停
        ui->pushButton_play->setIcon(QIcon(":/images/play.png"));
    }
    else
    {
        player->play();
        addToHistory();
        ui->pushButton_play->setIcon(QIcon(":/images/pause.png"));
    }
}

void MainWindow::on_pushButton_next_clicked()
{
    int index=playlist->currentIndex()==playlist->mediaCount()-1?
                0:
                playlist->nextIndex();
    playlist->setCurrentIndex(index);
    ui->listWidget_names->setCurrentRow(playlist->currentIndex());
    player->play();
    addToHistory();
}

void MainWindow::on_pushButton_history_clicked()
{
    HistoryDialog* hd = new HistoryDialog(this);
    hd->show();
}
