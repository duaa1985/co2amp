#include "mainwindow.h"


void MainWindow::UpdateControls()
{
    int i, index;
    bool bl;
    QString str;

    //QCoreApplication::processEvents(QEventLoop::AllEvents,1000);

    //////////////////////////// block signals ///////////////////////////
    BlockSignals(true);

    /////////// check if there is any active madium section among the optics //////////
    /*noam = true;
    QStringList list1, list2;
    list1 = Memorized.optics;//.split(QRegExp("[\n\r]"), QString::SkipEmptyParts);
    for(i=0; i<=list1.count()-1; i++){
        list2 = list1[i].split(QRegExp("[- \t\n]"), QString::SkipEmptyParts);
        if(list2.count() >= 2 && list2[1]=="AM")
            noam = false;
    }*/

    ////////////////////////////////// GUI CONTROLS //////////////////////////////////
    if(!flag_calculating){
        checkBox_saveWhenFinished->setChecked(0);
        checkBox_showCalculationTime->setChecked(0);
        //textBrowser->clear();
    }
    pushButton_new->setDisabled(flag_calculating);
    pushButton_open->setDisabled(flag_calculating);
    pushButton_save->setDisabled(flag_calculating || !flag_calculation_success || !(flag_plot_modified || flag_comments_modified || flag_results_modified) || project_file==QString());
    //pushButton_saveas->setDisabled(flag_calculating || !flag_calculation_success);
    pushButton_calculate->setDisabled(flag_calculating);
    textBrowser_terminal->setEnabled(flag_calculating); //terminal
    pushButton_abort->setEnabled(flag_calculating);
    checkBox_saveWhenFinished->setEnabled(flag_calculating && project_file!=QString());
    checkBox_showCalculationTime->setEnabled(flag_calculating);


    /////////////////////////////// OPTICS AND LAYOUT /////////////////////////////////
    int optic_count = listWidget_optics->count();
    int current_optic = listWidget_optics->currentRow();
    bl = optic_count > 0;
    if(bl)
        plainTextEdit_optic->setPlainText(Memorized.optic_yaml[current_optic]);
    else
        plainTextEdit_optic->setPlainText("");
    pushButton_optic_load->setEnabled(bl);
    pushButton_optic_save->setEnabled(bl);
    plainTextEdit_optic->setEnabled(bl);
    toolButton_optic_remove->setEnabled(bl);
    toolButton_optic_rename->setEnabled(bl);
    toolButton_optic_up->setEnabled(current_optic > 0);
    toolButton_optic_down->setEnabled(current_optic >= 0 && current_optic < optic_count-1);

    checkBox_noprop->setChecked(Memorized.noprop);


    /////////////////////////////////// CALCULATION NET //////////////////////////////////////


    if(!lineEdit_vc->hasFocus())
        lineEdit_vc->setText(Memorized.vc);
    if(!lineEdit_t_pulse_min->hasFocus())
        lineEdit_t_pulse_min->setText(Memorized.t_pulse_min);
    if(!lineEdit_t_pulse_max->hasFocus())
        lineEdit_t_pulse_max->setText(Memorized.t_pulse_max);
    comboBox_precision_t->setCurrentIndex(Memorized.precision_t);
    comboBox_precision_r->setCurrentIndex(Memorized.precision_r);
    double delta_t = (lineEdit_t_pulse_max->text().toDouble()-lineEdit_t_pulse_min->text().toDouble())/(comboBox_precision_t->currentText().toDouble()-1);
    double delta_v = 1.0/(lineEdit_t_pulse_max->text().toDouble()-lineEdit_t_pulse_min->text().toDouble());
    label_deltas->setText("(Δt = " + QString::number(delta_t) + " ps;   Δν = " + QString::number(delta_v) + " THz)");
    label_um->setText("(λ = " + QString::number(299.792458/lineEdit_vc->text().toDouble()) + " µm)"); // wl[um] = c[m/s] / nu[THz] * 1e-6

    /*comboBox_precision_t->setEnabled(!bl);
    comboBox_precision_r->setEnabled(!bl);
    lineEdit_t_pulse_min->setEnabled(!bl);
    lineEdit_t_pulse_max->setEnabled(!bl);*/


    /////////////////////// POPULATE COMBOBOXES IN THE OUTPUT TAB //////////////////////////////
    //optics
    //index = comboBox_optic->currentIndex();
    comboBox_optic->clear();
    //list1 = Saved.optic_id;//.split(QRegExp("[\n\r]"), QString::SkipEmptyParts);
    for(i=0; i<=Saved.optic_id.count()-1; i++){
        //list2 = list1[i].split(QRegExp("[- \t\n]"), QString::SkipEmptyParts);
        //if(list2.count() >= 1)
            comboBox_optic->addItem(Saved.optic_id[i]);
    }
    if(Memorized.optic == -1 || Memorized.optic+1 > comboBox_optic->count())
        comboBox_optic->setCurrentIndex(0);
    else
        comboBox_optic->setCurrentIndex(Memorized.optic);

    //pulse
    //index = comboBox_pulse->currentIndex();
    comboBox_pulse->clear();
    /*for(i=0; i<=Saved.n_pulses-1; i++)
        comboBox_pulse->addItem(QString::number(i+1));
    if(Memorized.pulse == -1 || Memorized.pulse+1 > comboBox_pulse->count())
        comboBox_pulse->setCurrentIndex(0);
    else
        comboBox_pulse->setCurrentIndex(Memorized.pulse);
    groupBox_pulse->setDisabled(Saved.n_pulses == 1); // Hide pulse combo if n_pulses==1*/

    //energyPlot
    index = comboBox_energyPlot->currentIndex();
    comboBox_energyPlot->clear();
    comboBox_energyPlot->addItem("all");
    comboBox_energyPlot->addItem("optic");
    /*if(Saved.n_pulses > 1){
        comboBox_energyPlot->addItem("pulse");
        comboBox_energyPlot->addItem("optic, pulse");
    }
    if(index == -1 || index+1 > comboBox_pulse->count()){
        if(Saved.n_pulses == 1) // single pulse
            comboBox_energyPlot->setCurrentIndex(1);
        else
            comboBox_energyPlot->setCurrentIndex(3);
    }
    else
        comboBox_energyPlot->setCurrentIndex(index);*/

    /////////////////////// ENABLE/DISABLE CONTROLS IN OUTPUT TAB //////////////////////////////
    bl = (flag_projectloaded || flag_calculation_success);
    //scrollArea_plotControls->setEnabled(bl);
    pushButton_update->setEnabled(bl);
    if(!bl)
        ClearPlot();

    bl = (comboBox_size->currentText()=="Custom");
    spinBox_width->setVisible(bl);
    spinBox_height->setVisible(bl);

    //////////////////////// unblock signals /////////////////////////////
    BlockSignals(false);
}


void MainWindow::BlockSignals(bool block)
{
    checkBox_noprop->blockSignals(block);
    listWidget_optics->blockSignals(block);
    plainTextEdit_optic->blockSignals(block);
    comboBox_precision_t->blockSignals(block);
    comboBox_precision_r->blockSignals(block);
}
