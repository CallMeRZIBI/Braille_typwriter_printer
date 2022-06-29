using System;

namespace translator{
    public static class Program{
        public static Dictionary<char, List<byte[]>> BrailleAlphabet = new Dictionary<char, List<byte[]>>(){
            { 'a', new List<byte[]>{new byte[]{1}} },
            { 'b', new List<byte[]>{new byte[]{1,2}} },
            { 'c', new List<byte[]>{new byte[]{1,4}} },
            { 'd', new List<byte[]>{new byte[]{1,4,5}} },
            { 'e', new List<byte[]>{new byte[]{1,5}} },
            { 'f', new List<byte[]>{new byte[]{1,2,4}} },
            { 'g', new List<byte[]>{new byte[]{1,2,4,5}} },
            { 'h', new List<byte[]>{new byte[]{1,2,5}} },
            { 'i', new List<byte[]>{new byte[]{2,4}} },
            { 'j', new List<byte[]>{new byte[]{2,4,5}} },
            { 'k', new List<byte[]>{new byte[]{1,3}} },
            { 'l', new List<byte[]>{new byte[]{1,2,3}} },
            { 'm', new List<byte[]>{new byte[]{1,3,4}} },
            { 'n', new List<byte[]>{new byte[]{1,3,4,5}} },
            { 'o', new List<byte[]>{new byte[]{1,3,5}} },
            { 'p', new List<byte[]>{new byte[]{1,2,3,4}} },
            { 'q', new List<byte[]>{new byte[]{1,2,3,4,5}} },
            { 'r', new List<byte[]>{new byte[]{1,2,3,5}} },
            { 's', new List<byte[]>{new byte[]{2,3,4}} },
            { 't', new List<byte[]>{new byte[]{2,3,4,5}} },
            { 'u', new List<byte[]>{new byte[]{1,3,6}} },
            { 'v', new List<byte[]>{new byte[]{1,2,3,6}} },
            { 'w', new List<byte[]>{new byte[]{2,4,5,6}} },
            { 'x', new List<byte[]>{new byte[]{1,3,4,6}} },
            { 'y', new List<byte[]>{new byte[]{1,3,4,5,6}} },
            { 'z', new List<byte[]>{new byte[]{1,3,5,6}} },
            { '$', new List<byte[]>{new byte[]{3,4,5,6}} },
            { '1', new List<byte[]>{new byte[]{3,4,5,6}, new byte[]{1}} },    // I shouldn't be doing numbers like that, so I need to change that
            { '2', new List<byte[]>{new byte[]{3,4,5,6}, new byte[]{1,2}} },
            { '3', new List<byte[]>{new byte[]{3,4,5,6}, new byte[]{1,4}} },
            { '4', new List<byte[]>{new byte[]{3,4,5,6}, new byte[]{1,4,5}} },
            { '5', new List<byte[]>{new byte[]{3,4,5,6}, new byte[]{1,5}} },
            { '6', new List<byte[]>{new byte[]{3,4,5,6}, new byte[]{1,2,4}} },
            { '7', new List<byte[]>{new byte[]{3,4,5,6}, new byte[]{1,2,4,5}} },
            { '8', new List<byte[]>{new byte[]{3,4,5,6}, new byte[]{1,2,5}} },
            { '9', new List<byte[]>{new byte[]{3,4,5,6}, new byte[]{2,4}} },
            { '0', new List<byte[]>{new byte[]{3,4,5,6}, new byte[]{2,4,5}} }
            };

        public static void Main(string[] args){
            string input = Console.ReadLine();
            input.ToLower();

            for(int i = 0; i< input.Length; i++){
                char ch = input[i];
                if(ch == ' '){
                    Console.Write(ch);
                    continue;
                }

                /*if(ch == '0'){
                    foreach(byte b in BrailleAlphabet['$']){
                        Console.Write(b);
                    }
                    Console.Write('|');
                    foreach(byte b in BrailleAlphabet['j']){
                        Console.Write(b);
                    }
                    Console.Write('|');
                    continue;
                }

                if(new char[]{'1','2','3','4','5','6','7','8','9'}.Contains(ch)){
                    foreach(byte b in BrailleAlphabet['$']){
                        Console.Write(b);
                    }
                    Console.Write('|');
                    ch = (char)((int)ch+48);
                }*/

                foreach(byte[] bytes in BrailleAlphabet[ch]){
                    foreach(byte b in bytes){
                        Console.Write(b);
                    }
                    Console.Write("|");
                }
            }
            Console.WriteLine();
        }
    }
}
