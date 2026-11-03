Import("env")

env.Replace(PROGNAME="RMS_S3_F1ATB_%s" % env.GetProjectOption("custom_prog_version"))