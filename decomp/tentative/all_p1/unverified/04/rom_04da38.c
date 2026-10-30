/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04da38.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

char candidate_4da38(void)

{
  char in_D0b;
  char cVar3;
  short sVar1;
  undefined2 uVar2;
  int extraout_A0;
  int unaff_A6;
  bool bVar4;

  bVar4 = false;
  if (*(char *)(unaff_A6 + 0x9e) == '\0') {
    in_D0b = candidate_1b04a();
    if (bVar4) {
      return in_D0b;
    }
    if (*(short *)(unaff_A6 + 0x84) != 0) {
      *(short *)(unaff_A6 + 0x84) = *(short *)(unaff_A6 + 0x84) + -1;
      return in_D0b;
    }
    *(undefined2 *)(unaff_A6 + 0x84) = *(undefined2 *)(unaff_A6 + 0x80);
    if (*(char *)(unaff_A6 + 0xa1) == '\0') {
      candidate_1b04a();
      *(int *)(unaff_A6 + 0x6e) = extraout_A0;
      if (*(short *)(extraout_A0 + 0x3c) < (short)(*(short *)(unaff_A6 + 0x3c) + -0x40)) {
        if (extraout_A0 == 0x1014c4) {
          cVar3 = candidate_aaa88();
          if (cVar3 == '\0') {
            if (*(char *)(unaff_A6 + 0xa4) == '\0') {
              sVar1 = candidate_260c8();
            }
            else {
              sVar1 = candidate_260c8();
            }
          }
          else if (*(char *)(unaff_A6 + 0xa4) == '\0') {
            sVar1 = candidate_260c8();
          }
          else {
            sVar1 = candidate_260c8();
          }
        }
        else {
          cVar3 = candidate_aaa88();
          if (cVar3 == '\0') {
            if (*(char *)(unaff_A6 + 0xa4) == '\0') {
              sVar1 = candidate_260c8();
            }
            else {
              sVar1 = candidate_260c8();
            }
          }
          else if (*(char *)(unaff_A6 + 0xa4) == '\0') {
            sVar1 = candidate_260c8();
          }
          else {
            sVar1 = candidate_260c8();
          }
        }
        if (7 < sVar1) {
          sVar1 = 0;
        }
        in_D0b = *(char *)(sVar1 + 0x4dc54);
        if (in_D0b == '\x04') {
          *(byte *)(unaff_A6 + 0xa0) = *(byte *)(unaff_A6 + 0xa0) | 0x80;
          *(byte *)(unaff_A6 + 0xa0) = *(byte *)(unaff_A6 + 0xa0) | 0x10;
        }
      }
      else if (*(short *)(extraout_A0 + 0x40) < (short)(*(short *)(unaff_A6 + 0x40) + 0x60)) {
        in_D0b = '\x04';
      }
      else {
        in_D0b = '\x05';
      }
      if (in_D0b == '\a') {
        bVar4 = false;
        candidate_4dd86();
        if (bVar4) {
          return '\a';
        }
      }
      if (in_D0b == '\x04') {
        *(undefined1 *)(unaff_A6 + 0xa1) = 1;
        if (*(char *)(unaff_A6 + 0xa4) == '\0') {
          uVar2 = candidate_260c8();
          *(undefined2 *)(unaff_A6 + 0x26) = uVar2;
        }
        else {
          uVar2 = candidate_260c8();
          *(undefined2 *)(unaff_A6 + 0x26) = uVar2;
        }
        *(undefined2 *)(unaff_A6 + 0x7e) = *(undefined2 *)(unaff_A6 + 0x26);
      }
      *(char *)(unaff_A6 + 0x9d) = in_D0b;
      *(undefined1 *)(unaff_A6 + 0x9e) = 0xff;
      if (in_D0b == '\b') {
        *(undefined1 *)(unaff_A6 + 0x9e) = 0;
        return '\b';
      }
      if (in_D0b == '\x04') {
        *(byte *)(unaff_A6 + 0xa0) = *(byte *)(unaff_A6 + 0xa0) | 0x10;
        *(undefined1 *)(unaff_A6 + 0x9e) = 0;
        return '\x04';
      }
    }
    else {
      *(undefined1 *)(unaff_A6 + 0xa1) = 0;
      *(undefined1 *)(unaff_A6 + 0x9d) = 3;
      *(undefined1 *)(unaff_A6 + 0x9e) = 0xff;
    }
  }
  return in_D0b;
}
