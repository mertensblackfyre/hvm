let
  pkgs = import <nixpkgs> {};
in
  pkgs.mkShell {
     packages = [
      pkgs.gtest
      pkgs.cmake
      pkgs.meson
      pkgs.ninja
      pkgs.gcc
    ];
  }
