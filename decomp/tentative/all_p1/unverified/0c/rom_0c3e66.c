/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c3e66.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c3e66(void)

{
  char cVar1;
  byte bVar2;
  uint uVar3;
  short unaff_D7w;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  int unaff_A6;
  bool bVar4;

  uVar3 = candidate_c0fea();
  if ((uVar3 & 0x40) != 0) {
LAB_000c3bcc:
    *(byte *)(unaff_A6 + 0x9a) = *(byte *)(unaff_A6 + 0x9a) & 0xdf;
    *(undefined1 *)(unaff_A6 + 0x98) = 2;
    *(byte *)(unaff_A6 + 0x9b) = *(byte *)(unaff_A6 + 0x9b) & 0xbf;
    bVar4 = false;
    *(byte *)(unaff_A6 + 0x9c) = *(byte *)(unaff_A6 + 0x9c) & 0xbf;
    candidate_c487a();
    if (bVar4) {
      candidate_c0fce();
    }
    else {
      *(undefined1 *)(unaff_A6 + 0x92) = 0;
      *(undefined1 *)(unaff_A6 + 0x76) = 0;
      candidate_c0f62();
    }
LAB_000c3d74:
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
        *(ushort *)(extraout_A0_00 + 4) = *(ushort *)(extraout_A0_00 + 4) | 2;
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
  if ((*(char *)(unaff_A6 + 0x76) == '\0') ||
     (bVar2 = *(byte *)(unaff_A6 + 0x9b), *(byte *)(unaff_A6 + 0x9b) = bVar2 & 0xef,
     (bVar2 & 0x10) == 0)) {
LAB_000c3e88:
    candidate_c3778();
    if (unaff_D7w != 0) {
      *(code **)(unaff_A6 + 0x7c) = candidate_c38b2;
    }
    return;
  }
  do {
    *(byte *)(unaff_A6 + 0x9b) = *(byte *)(unaff_A6 + 0x9b) & 0xef;
    *(byte *)(unaff_A6 + 0x9c) = *(byte *)(unaff_A6 + 0x9c) & 0xef;
    *(byte *)(unaff_A6 + 0x9a) = *(byte *)(unaff_A6 + 0x9a) & 0xdf;
    *(undefined1 *)(unaff_A6 + 0x98) = 2;
    if ((*(char *)(unaff_A6 + 0x93) == '\x03') || ((*(byte *)(unaff_A6 + 0x9a) & 1) == 0)) {
      cVar1 = *(char *)(*(short *)(unaff_A6 + 0x70) + 0x10c71e);
      if ((cVar1 == '\x05') || (cVar1 == '\n')) {
        *(byte *)(unaff_A6 + 0x9b) = *(byte *)(unaff_A6 + 0x9b) & 0xef;
        *(byte *)(unaff_A6 + 0x9c) = *(byte *)(unaff_A6 + 0x9c) & 0xef;
        if ((*(byte *)(unaff_A6 + 0x9a) & 1) == 0) {
          *(undefined1 *)(unaff_A6 + 0x98) = 2;
          uVar3 = candidate_c0fea();
          if ((uVar3 & 1) == 0) {
            if ((uVar3 & 2) == 0) {
              cVar1 = *(char *)(unaff_A6 + 0x92);
              if (*(char *)(unaff_A6 + 0x92) < '\x01') goto joined_r0x000c3efc;
            }
            else {
              if (*(char *)(unaff_A6 + 0x92) == '\0') {
                *(undefined1 *)(unaff_A6 + 0x92) = 0xff;
                goto LAB_000c3fde;
              }
              if (*(char *)(unaff_A6 + 0x92) < '\0') goto LAB_000c3f04;
            }
            *(undefined1 *)(unaff_A6 + 0x92) = 0;
          }
          else {
            cVar1 = *(char *)(unaff_A6 + 0x92);
            if (*(char *)(unaff_A6 + 0x92) == '\0') {
              *(undefined1 *)(unaff_A6 + 0x92) = 1;
              goto LAB_000c3fde;
            }
joined_r0x000c3efc:
            if (-1 < cVar1) {
LAB_000c3f04:
              candidate_c0f62();
              *(undefined1 *)(unaff_A6 + 0x76) = 0;
              *(code **)(unaff_A6 + 0x7c) = candidate_c3f1a;
              candidate_c3778();
              if (unaff_D7w != 0) {
                bVar2 = *(byte *)(unaff_A6 + 0x9b);
                *(byte *)(unaff_A6 + 0x9b) = bVar2 & 0xef;
                if ((bVar2 & 0x10) == 0) {
                  *(code **)(unaff_A6 + 0x7c) = candidate_c38b2;
                }
                else {
                  *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3cd0;
                }
              }
              return;
            }
            *(undefined1 *)(unaff_A6 + 0x92) = 0;
          }
LAB_000c3fde:
          candidate_c0f62();
          *(byte *)(unaff_A6 + 0x9b) = *(byte *)(unaff_A6 + 0x9b) & 0xef;
          *(byte *)(unaff_A6 + 0x9c) = *(byte *)(unaff_A6 + 0x9c) & 0xef;
          *(code **)(unaff_A6 + 0x7c) = candidate_c3ffa;
          candidate_c3778();
          if (unaff_D7w != 0) {
            bVar2 = *(byte *)(unaff_A6 + 0x9b);
            *(byte *)(unaff_A6 + 0x9b) = bVar2 & 0xef;
            if ((bVar2 & 0x10) == 0) {
              *(code **)(unaff_A6 + 0x7c) = candidate_c38b2;
            }
            else if (*(char *)(unaff_A5 + 0x471e + (int)*(short *)(unaff_A6 + 0x70)) == '\x05') {
              if (*(char *)(unaff_A5 + 0x471e + (int)*(short *)(unaff_A6 + 0x70)) == '\n') {
                uVar3 = candidate_c0fea();
                if ((uVar3 & 1) == 0) {
                  if ((uVar3 & 2) == 0) {
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
      }
      else {
        if ((cVar1 != '\x06') && (cVar1 != '\v')) {
          uVar3 = candidate_c0fea();
          if ((uVar3 & 1) == 0) {
            if ((uVar3 & 2) == 0) {
              *(undefined1 *)(unaff_A6 + 0x92) = 0;
            }
            else {
              *(undefined1 *)(unaff_A6 + 0x92) = 0xff;
            }
          }
          else {
            *(undefined1 *)(unaff_A6 + 0x92) = 1;
          }
          candidate_c0f62();
          goto LAB_000c3d74;
        }
        *(byte *)(unaff_A6 + 0x9b) = *(byte *)(unaff_A6 + 0x9b) & 0xef;
        *(byte *)(unaff_A6 + 0x9c) = *(byte *)(unaff_A6 + 0x9c) & 0xef;
        if ((*(byte *)(unaff_A6 + 0x9a) & 1) == 0) {
          *(undefined1 *)(unaff_A6 + 0x98) = 2;
          uVar3 = candidate_c0fea();
          *(undefined1 *)(unaff_A6 + 0x92) = 0;
          if ((uVar3 & 1) != 0) {
            *(undefined1 *)(unaff_A6 + 0x92) = 1;
          }
          if ((uVar3 & 2) != 0) {
            *(undefined1 *)(unaff_A6 + 0x92) = 0xff;
          }
          candidate_c0f62();
          *(undefined1 *)(unaff_A6 + 0x76) = 0;
          *(code **)(unaff_A6 + 0x7c) = candidate_c3e66;
          goto LAB_000c3e88;
        }
      }
    }
    *(byte *)(unaff_A6 + 0x9a) = *(byte *)(unaff_A6 + 0x9a) & 0xdf;
    *(undefined1 *)(unaff_A6 + 0x92) = 0;
    *(undefined1 *)(unaff_A6 + 0x98) = 1;
    *(undefined1 *)(unaff_A6 + 0x92) = 0;
    *(byte *)(unaff_A6 + 0x9a) = *(byte *)(unaff_A6 + 0x9a) ^ 2;
    candidate_c0f62();
    *(undefined1 *)(unaff_A6 + 0x76) = 0;
    *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3b82;
    uVar3 = candidate_c0fea();
    if (*(char *)(unaff_A6 + 0x76) == '\0') {
      candidate_ca2ce();
      *(ushort *)(extraout_A0 + 4) = *(ushort *)(extraout_A0 + 4) | 2;
      goto LAB_000c3bb8;
    }
    *(undefined1 *)(unaff_A6 + 0x98) = 0;
    if ((uVar3 & 0x40) != 0) goto LAB_000c3bcc;
    if ((uVar3 & 0x10) == 0) {
LAB_000c3bb8:
      candidate_c3778();
      if (unaff_D7w != 0) {
        *(code **)(unaff_A6 + 0x7c) = candidate_c38b2;
      }
      return;
    }
  } while( true );
}
