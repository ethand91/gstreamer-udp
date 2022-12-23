#include <gst/gst.h>

int main(int argc, char *argv[])
{
  gst_init(&argc, &argv);

  GstElement *pipeline, *source, *vp8enc, *vorbisenc, *muxer, *sink;

  pipeline = gst_pipeline_new("pipeline");

  source = gst_element_factory_make("udpsrc", "udpsrc");
  g_object_set(source, "port", 3001, NULL);

  vp8enc = gst_element_factory_make("vp8enc", "vp8enc");
  vorbisenc = gst_element_factory_make("vorbisenc", "vorbisenc");
  muxer = gst_element_factory_make("webmmux", "webmmux");
  sink = gst_element_factory_make("filesink", "filesink");
  g_object_set(sink, "location", "output.webm", NULL);

  gst_bin_add_many(GST_BIN(pipeline), source, vp8enc, vorbisenc, muxer, sink, NULL);
  gst_element_link_many(source, vp8enc, vorbisenc, muxer, sink, NULL);

  GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

  if (ret == GST_STATE_CHANGE_FAILURE)
  {
    g_printerr("Unable to set the pipeline to playing state\n");

    return -1;
  }

  g_print("Pipeline set to playing\n");
  g_print("Listening for UDP on udp://0.0.0.0:3001\n");

  GstBus *bus = gst_element_get_bus(pipeline);
  GstMessage *message = gst_bus_timed_pop_filtered(
    bus,
    GST_CLOCK_TIME_NONE,
    static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS)
  );

  if (message != NULL)
  {
    GError *error;
    gst_message_parse_error(message, &error, NULL);
    g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(message->src), error->message);
    g_clear_error(&error);
    gst_message_unref(message);
  }

  gst_object_unref(bus);
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);

  return 0;
}
