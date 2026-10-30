/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c3ffa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c3ffa(void)

{
  byte bVar1;
  uint uVar2;
  short unaff_D7w;
  int extraout_A0;
  int unaff_A5;
  int unaff_A6;
  bool bVar3;

  uVar2 = candidate_c0fea();
  if ((uVar2 & 0x40) != 0) {
    *(byte *)(unaff_A6 + 0x9a) = *(byte *)(unaff_A6 + 0x9a) & 0xdf;
    *(undefined1 *)(unaff_A6 + 0x98) = 2;
    *(byte *)(unaff_A6 + 0x9b) = *(byte *)(unaff_A6 + 0x9b) & 0xbf;
    bVar3 = false;
    *(byte *)(unaff_A6 + 0x9c) = *(byte *)(unaff_A6 + 0x9c) & 0xbf;
    candidate_c487a();
    if (bVar3) {
      candidate_c0fce();
    }
    else {
      *(undefined1 *)(unaff_A6 + 0x92) = 0;
      *(undefined1 *)(unaff_A6 + 0x76) = 0;
      candidate_c0f62();
    }
    *(byte *)(unaff_A6 + 0x9b) = *(byte *)(unaff_A6 + 0x9b) & 0xef;
    *(byte *)(unaff_A6 + 0x9c) = *(byte *)(unaff_A6 + 0x9c) & 0xef;
    *(byte *)(unaff_A6 + 0x9b) = *(byte *)(unaff_A6 + 0x9b) & 0xbf;
    *(byte *)(unaff_A6 + 0x9c) = *(byte *)(unaff_A6 + 0x9c) & 0xbf;
    *(undefined1 *)(unaff_A6 + 0x76) = 0;
    *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3d9a;
    candidate_c0fea();
    if (*(char *)(unaff_A6 + 0x76) == '\0') {
      if ((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) {
        candidate_ca2ce();
        *(ushort *)(extraout_A0 + 4) = *(ushort *)(extraout_A0 + 4) | 2;
      }
    }
    else {
      *(undefined1 *)(unaff_A6 + 0x98) = 0;
      if ((*(byte *)(unaff_A6 + 0x9b) & 0x10) != 0) {
        *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3cd0;
      }
      if ((*(byte *)(unaff_A6 + 0x9b) & 0x40) != 0) {
        *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3bcc;
      }
    }
    candidate_c3778();
    if (unaff_D7w != 0) {
      *(code **)(unaff_A6 + 0x7c) = candidate_c38b2;
    }
    return;
  }
  candidate_c3778();
  if (unaff_D7w != 0) {
    bVar1 = *(byte *)(unaff_A6 + 0x9b);
    *(byte *)(unaff_A6 + 0x9b) = bVar1 & 0xef;
    if ((bVar1 & 0x10) == 0) {
      *(code **)(unaff_A6 + 0x7c) = candidate_c38b2;
    }
    else if (*(char *)(unaff_A5 + 0x471e + (int)*(short *)(unaff_A6 + 0x70)) == '\x05') {
      if (*(char *)(unaff_A5 + 0x471e + (int)*(short *)(unaff_A6 + 0x70)) == '\n') {
        uVar2 = candidate_c0fea();
        if ((uVar2 & 1) == 0) {
          if ((uVar2 & 2) == 0) {
            if (*(char *)(unaff_A6 + 0x92) < '\x01') {
              if (*(char *)(unaff_A6 + 0x92) < '\0') {
                *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3fd0;
              }
              else {
                *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3cd0;
              }
            }
            else {
              *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3fc2;
            }
          }
          else if (*(char *)(unaff_A6 + 0x92) == '\0') {
            *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3fa6;
          }
          else if (*(char *)(unaff_A6 + 0x92) < '\0') {
            *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3cd0;
          }
          else {
            *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3fc2;
          }
        }
        else if (*(char *)(unaff_A6 + 0x92) == '\0') {
          *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3fb4;
        }
        else if (*(char *)(unaff_A6 + 0x92) < '\0') {
          *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3fd0;
        }
        else {
          *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3cd0;
        }
      }
      else {
        *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3cd0;
      }
    }
    else {
      *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3cd0;
    }
  }
  return;
}
