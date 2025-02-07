namespace Editor.Utilities
{
    class LogMessage
    {
        public DateTime Time { get; }
        public MessageType Type { get; }
        public string Message { get; }
        public string File { get; }
        public string Caller { get; }
        public int Line { get; }
        public string MetaData => $"{File} : {Caller} ({Line})";

        public LogMessage(MessageType type, string msg, string file, string caller, int line)
        {
            Time = DateTime.Now;
            Type = type;
            Message = msg;
            File = System.IO.Path.GetFileName(file);
            Caller = caller;
            Line = line;
        }
    }
}
