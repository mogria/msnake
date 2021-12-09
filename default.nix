{ pkgs ? import <nixpkgs> {} }:

let
  stdenv = pkgs.stdenv;
in stdenv.mkDerivation {
  pname = "msnake-env";
  version = "1.0";

  buildInputs = with pkgs; [
    cmake
    gnumake
    ncurses
    check
  ];
}
