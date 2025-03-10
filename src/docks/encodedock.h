/*
 * Copyright (c) 2012-2024 Meltytech, LLC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ENCODEDOCK_H
#define ENCODEDOCK_H

#include "settings.h"

#include <MltProperties.h>
#include <QDockWidget>
#include <QDomElement>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QStringList>

class QTreeWidgetItem;
class QTemporaryFile;
namespace Ui {
class EncodeDock;
}
class AbstractJob;
class MeltJob;
namespace Mlt {
class Service;
class Producer;
class Filter;
} // namespace Mlt

class PresetsProxyModel : public QSortFilterProxyModel
{
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

class EncodeDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit EncodeDock(QWidget *parent = 0);
    ~EncodeDock();

    void loadPresetFromProperties(Mlt::Properties &);
    bool isExportInProgress() const;

signals:
    void captureStateChanged(bool);
    void createOrEditFilterOnOutput(Mlt::Filter *, const QStringList & = {});

public slots:
    void onAudioChannelsChanged();
    void onProducerOpened();
    void onProfileChanged();
    void onReframeChanged();
    void on_hwencodeButton_clicked();
    bool detectHardwareEncoders();

private slots:
    void on_presetsTree_clicked(const QModelIndex &index);
    void on_presetsTree_activated(const QModelIndex &index);

    void on_encodeButton_clicked();

    void on_streamButton_clicked();

    void on_addPresetButton_clicked();

    void on_removePresetButton_clicked();

    void onFinished(AbstractJob *, bool isSuccess);

    void on_stopCaptureButton_clicked();

    void on_videoRateControlCombo_activated(int index);

    void on_audioRateControlCombo_activated(int index);

    void on_scanModeCombo_currentIndexChanged(int index);

    void on_presetsSearch_textChanged(const QString &search);

    void on_resetButton_clicked();

    void openCaptureFile();

    void on_formatCombo_currentIndexChanged(int index);

    void on_videoBufferDurationChanged();

    void on_gopSpinner_valueChanged(int value);

    void on_fromCombo_currentIndexChanged(int index);

    void on_videoCodecCombo_currentIndexChanged(int index);

    void on_audioCodecCombo_currentIndexChanged(int index);

    void setAudioChannels(int channels);

    void on_widthSpinner_editingFinished();

    void on_heightSpinner_editingFinished();

    void on_advancedButton_clicked(bool checked);

    void on_hwencodeCheckBox_clicked(bool checked);

    void on_advancedCheckBox_clicked(bool checked);

    void on_fpsSpinner_editingFinished();

    void on_fpsComboBox_activated(int arg1);

    void on_videoQualitySpinner_valueChanged(int vq);

    void on_audioQualitySpinner_valueChanged(int aq);

    void on_parallelCheckbox_clicked(bool checked);

    void on_resolutionComboBox_activated(int arg1);

    void on_reframeButton_clicked();

    void on_aspectNumSpinner_valueChanged(int value);

    void on_aspectDenSpinner_valueChanged(int value);

private:
    enum {
        RateControlAverage = 0,
        RateControlConstant,
        RateControlQuality,
        RateControlConstrained
    };
    enum {
        AudioChannels1 = 0,
        AudioChannels2,
        AudioChannels4,
        AudioChannels6,
    };
    Ui::EncodeDock *ui;
    Mlt::Properties *m_presets;
    QScopedPointer<MeltJob> m_immediateJob;
    QString m_extension;
    Mlt::Properties *m_profiles;
    PresetsProxyModel m_presetsModel;
    QStringList m_outputFilenames;
    bool m_isDefaultSettings;
    double m_fps;
    QStringList m_intraOnlyCodecs;
    QStringList m_losslessVideoCodecs;
    QStringList m_losslessAudioCodecs;

    void loadPresets();
    Mlt::Properties *collectProperties(int realtime, bool includeProfile = false);
    void collectProperties(QDomElement &node, int realtime);
    void setSubtitleProperties(QDomElement &node, Mlt::Producer *service);
    QPoint addConsumerElement(
        Mlt::Producer *service, QDomDocument &dom, const QString &target, int realtime, int pass);
    MeltJob *convertReframe(Mlt::Producer *service,
                            QTemporaryFile *tmp,
                            const QString &target,
                            int realtime,
                            int pass,
                            const QThread::Priority priority);
    MeltJob *createMeltJob(Mlt::Producer *service,
                           const QString &target,
                           int realtime,
                           int pass = 0,
                           const QThread::Priority priority = Settings.jobPriority());
    void runMelt(const QString &target, int realtime = -1);
    void enqueueAnalysis();
    void enqueueMelt(const QStringList &targets, int realtime);
    void encode(const QString &target);
    void resetOptions();
    Mlt::Producer *fromProducer(bool usePlaylistBin = false) const;
    static void filterCodecParams(const QString &vcodec, QStringList &other);
    void onVideoCodecComboChanged(int index, bool ignorePreset = false, bool resetBframes = true);
    bool checkForMissingFiles();
    QString &defaultFormatExtension();
    void initSpecialCodecLists();
    void setReframeEnabled(bool enabled);
    void showResampleWarning(const QString &message);
    void hideResampleWarning(bool hide = true);
    void checkFrameRate();
    void setResolutionAspectFromProfile();
};

#endif // ENCODEDOCK_H
