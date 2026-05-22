cmake_minimum_required(VERSION 3.22)
project(SaarthalDelay VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.4
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(SaarthalDelay
    COMPANY_NAME                "OJT"
    PLUGIN_MANUFACTURER_CODE    Ojta
    PLUGIN_CODE                 Sdly
    FORMATS                     VST3 Standalone
    PRODUCT_NAME                "Saarthal Delay"
    IS_SYNTH                    FALSE
    NEEDS_MIDI_INPUT            FALSE
    NEEDS_MIDI_OUTPUT           FALSE
    IS_MIDI_EFFECT              FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD     FALSE
    VST3_CATEGORIES             "Fx" "Delay"
)

target_sources(SaarthalDelay PRIVATE
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
    src/dsp/DigitalDelay.cpp
    src/dsp/TapeDelay.cpp
    src/dsp/AnalogDelay.cpp
    src/dsp/DelayLine.cpp
    src/ui/KnobLookAndFeel.cpp
)

target_include_directories(SaarthalDelay PRIVATE src)

target_compile_definitions(SaarthalDelay PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(SaarthalDelay PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
    juce::juce_gui_extra
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

juce_generate_juce_header(SaarthalDelay)
