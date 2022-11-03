## cl89
## Copyright 2022 kmx.io <contact@kmx.io>
##
## Permission is hereby granted to use this software granted
## the above copyright notice and this permission paragraph
## are included in all copies and substantial portions of this
## software.
##
## THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
## PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
## AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
## THIS SOFTWARE.

defmodule Header.C do
  def split(src) do
    split(src, "")
  end

  def split("/*" <> rest, "") do
    split(rest, "/*")
  end
  def split("/*" <> rest, acc) do
    {"", acc <> "/*" <> rest}
  end
  def split("*/\n" <> rest, acc) do
    header = acc <> "*/"
    {header, rest}
  end
  def split(<<c, rest::binary>>, acc) when is_binary(acc) do
    split(rest, acc <> <<c>>)
  end
  def split("", acc) do
    {"", acc}
  end

  def main([src_path | dest_paths]) do
    case File.read(src_path) do
      {:ok, src} ->
        {header, _} = split(src)
        Enum.each dest_paths, fn dest_path ->
          {:ok, dest} = File.read(dest_path)
          {_, rest} = split(dest)
          result = header <> "\n" <> rest
          File.write(dest_path, result)
        end
      {:error, e} ->
        IO.inspect "Error: #{src_path}: #{e}"
    end
  end
end

defmodule Header.Make do
  def split(src) do
    split(src, [])
  end

  def split([line = ("#" <> _) | rest], acc) do
    split(rest, [line | acc])
  end
  def split(rest, acc) do
    {Enum.reverse(acc) |> Enum.join("\n"), rest |> Enum.join("\n")}
  end

  def main([src_path | dest_paths]) do
    {:ok, src} = File.read(src_path)
    src_lines = String.split(src, "\n")
    {header, _} = split(src_lines)
    Enum.each dest_paths, fn dest_path ->
      {:ok, dest} = File.read(dest_path)
      dest_lines = String.split(dest, "\n")
      {_, rest} = split(dest_lines)
      result = header <> "\n" <> rest
      File.write(dest_path, result)
    end
  end
end

src_path = hd(System.argv)
module = case src_path do
           "Makefile" -> Header.Make
           "configure" -> Header.Make
           "config.subr" -> Header.Make
           _ ->
             case Regex.run(~r/[.][ch]$/, src_path) do
               [_] -> Header.C
               _ -> raise "error"
             end
         end
module.main(System.argv)
