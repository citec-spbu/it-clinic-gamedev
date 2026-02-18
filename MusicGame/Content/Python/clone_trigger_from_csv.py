import unreal
import csv
import os

LOG = lambda s: unreal.log("[TriggerClone] " + str(s))
WARN = lambda s: unreal.log_warning("[TriggerClone] " + str(s))


def read_csv_times(csv_path):
    times = []
    with open(csv_path, newline='') as f:
        for row in csv.reader(f):
            if not row:
                continue
            try:
                times.append(float(row[0]))
            except:
                continue
    return sorted(times)


def run(sequence_path, csv_path):

    seq = unreal.load_asset(sequence_path)
    if not seq:
        raise Exception("Sequence not found")

    LOG("Sequence: " + seq.get_name())

    # --- найти EventTrack ---
    ext = unreal.MovieSceneSequenceExtensions
    tracks = ext.get_tracks(seq)

    event_track = None
    for t in tracks:
        if t.get_class().get_name() == "MovieSceneEventTrack":
            event_track = t
            break

    if not event_track:
        raise Exception("EventTrack not found")

    LOG("EventTrack found")

    # --- взять первую секцию ---
    sections = event_track.get_sections()
    if not sections:
        raise Exception("No sections in EventTrack")

    section = sections[0]
    LOG("Section: " + section.get_class().get_name())

    # --- получить channel ---
    channels = section.get_all_channels()
    if not channels:
        raise Exception("No channels found in section")

    channel = channels[0]
    LOG("Channel: " + channel.get_class().get_name())

    # --- получить ключи ---
    keys = channel.get_keys()
    if not keys:
        raise Exception("No keys found in channel")

    template_key = keys[0]
    template_value = template_key.get_value()

    LOG("Template key found")

    # --- читать CSV ---
    times = read_csv_times(csv_path)
    LOG(f"Times from CSV: {len(times)}")

    display_rate = seq.get_display_rate()

    added = 0

    for t in times:
        frame_number = unreal.FrameNumber(
            int(t * display_rate.numerator / display_rate.denominator)
        )

        channel.add_key(frame_number, template_value)
        added += 1

    LOG(f"Added keys: {added}")

    unreal.EditorAssetLibrary.save_asset(sequence_path)
    LOG("Sequence saved")

