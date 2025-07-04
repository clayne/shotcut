/*
 * Copyright (c) 2015-2025 Meltytech, LLC
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
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Shotcut.Controls as Shotcut

Item {
    function setControls() {
        sliderDelay.value = filter.getDouble('0');
        sliderFeedback.value = filter.getDouble('1');
        sliderWetness.value = filter.getDouble('wetness') * sliderWetness.maximumValue;
        channelMask.setChannelsControls();
    }

    width: 350
    height: 125
    Component.onCompleted: {
        if (filter.isNew) {
            // Set default parameter values
            filter.set('0', 1);
            filter.set('1', -10);
            filter.set('wetness', 1);
            filter.savePreset(preset.parameters);
        }
        setControls();
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 8
        columns: 3

        Label {
            text: qsTr('Preset')
            Layout.alignment: Qt.AlignRight
        }

        Shotcut.Preset {
            id: preset

            parameters: ['0', '1', 'wetness', channelMask.channelMaskProperty]
            Layout.columnSpan: 2
            onPresetSelected: setControls()
        }

        Label {
            text: qsTr('Delay')
            Layout.alignment: Qt.AlignRight

            Shotcut.HoverTip {
                text: qsTr('The neutral delay time is 2 seconds.\nTimes above 2 seconds will have reduced quality.\nTimes below will have increased CPU usage.')
            }
        }

        Shotcut.SliderSpinner {
            id: sliderDelay

            minimumValue: 0
            maximumValue: 4
            suffix: ' s'
            decimals: 2
            value: filter.getDouble('0')
            onValueChanged: {
                filter.set('0', value);
            }
        }

        Shotcut.UndoButton {
            onClicked: sliderDelay.value = 1
        }

        Label {
            text: qsTr('Feedback')
            Layout.alignment: Qt.AlignRight
        }

        Shotcut.SliderSpinner {
            id: sliderFeedback

            minimumValue: -70
            maximumValue: 0
            suffix: ' dB'
            decimals: 1
            value: filter.getDouble('1')
            onValueChanged: {
                filter.set('1', value);
            }
        }

        Shotcut.UndoButton {
            onClicked: sliderFeedback.value = -10
        }

        Label {
            text: qsTr('Dry')
            Layout.alignment: Qt.AlignRight
        }

        Shotcut.SliderSpinner {
            id: sliderWetness

            minimumValue: 0
            maximumValue: 100
            decimals: 1
            label: qsTr('Wet')
            suffix: ' %'
            value: filter.getDouble('wetness') * maximumValue
            onValueChanged: {
                filter.set('wetness', value / maximumValue);
            }
        }

        Shotcut.UndoButton {
            onClicked: sliderWetness.value = sliderWetness.maximumValue
        }

        Label {
            text: qsTr('Channels')
            Layout.alignment: Qt.AlignRight
        }

        Shotcut.ChannelMask {
            id: channelMask

            Layout.columnSpan: 2
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
