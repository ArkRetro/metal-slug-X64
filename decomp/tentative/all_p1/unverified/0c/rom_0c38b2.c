/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c38b2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c38b2(void)

{
  char cVar1;
  uint uVar2;
  byte bVar4;
  ushort uVar3;
  uint unaff_D2;
  short unaff_D7w;
  undefined2 uVar5;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  int unaff_A6;
  bool bVar6;

  *(undefined1 *)(unaff_A6 + 0x98) = 0;
  *(undefined1 *)(unaff_A6 + 0x91) = 6;
LAB_000c3a26:
  if ((*(char *)(unaff_A6 + 0x93) == '\x02') &&
     (bVar4 = *(byte *)(unaff_A6 + 0x9a), *(byte *)(unaff_A6 + 0x9a) = bVar4 & 0xef,
     (bVar4 & 0x10) != 0)) {
    candidate_c0fce();
    *(undefined1 *)(unaff_A6 + 0x91) = 4;
  }
  else {
    candidate_c0f62();
  }
  do {
    *(undefined1 *)(unaff_A6 + 0x97) = 0;
    *(undefined4 *)(unaff_A6 + 0x7c) = 0xc3a5e;
    uVar2 = candidate_c0fea();
    if ((uVar2 & 0x40) != 0) goto LAB_000c3bcc;
    if ((uVar2 & 0x10) != 0) break;
    if (*(char *)(unaff_A6 + 0x92) != '\0') {
      if (*(char *)(unaff_A6 + 0x92) < '\0') {
        if ((uVar2 & 1) == 0) {
          if ((uVar2 & 2) != 0) goto LAB_000c3ac4;
          *(undefined1 *)(unaff_A6 + 0x92) = 0;
          candidate_c0f62();
        }
        else {
          *(undefined1 *)(unaff_A6 + 0x92) = 1;
          candidate_c0f62();
        }
      }
      else if ((uVar2 & 2) == 0) {
        if ((uVar2 & 1) != 0) goto LAB_000c3ac4;
        *(undefined1 *)(unaff_A6 + 0x92) = 0;
        candidate_c0f62();
      }
      else {
        *(undefined1 *)(unaff_A6 + 0x92) = 0xff;
        candidate_c0f62();
      }
LAB_000c4168:
      *(undefined1 *)(unaff_A6 + 0x98) = 1;
      *(undefined4 *)(unaff_A6 + 0x7c) = 0xc4176;
      uVar2 = candidate_c0fea();
      if ((uVar2 & 0x40) != 0) goto LAB_000c3bcc;
      if ((uVar2 & 0x10) == 0) {
        candidate_c3778();
        if (unaff_D7w != 0) {
          *(code **)(unaff_A6 + 0x7c) = candidate_c397e;
        }
        return;
      }
      break;
    }
    if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
      if ((uVar2 & 2) != 0) {
        *(undefined1 *)(unaff_A6 + 0x92) = 0xff;
        candidate_c0f62();
        goto LAB_000c4168;
      }
    }
    else if ((uVar2 & 1) != 0) {
      *(undefined1 *)(unaff_A6 + 0x92) = 1;
      candidate_c0f62();
      goto LAB_000c4168;
    }
LAB_000c3ac4:
    if (((*(char *)(unaff_A6 + 0x92) == '\0') &&
        (bVar4 = (*(byte *)(unaff_A6 + 0x97) + 1) - (0xfe < *(byte *)(unaff_A6 + 0x97)),
        *(byte *)(unaff_A6 + 0x97) = bVar4, 3 < bVar4)) &&
       (uVar3 = candidate_29c6(), uVar3 < *(byte *)(unaff_A6 + 0x97))) {
      *(undefined1 *)(unaff_A6 + 0x97) = 0;
      bVar6 = false;
      candidate_c0f96();
      if (((bVar6) && (*(char *)(unaff_A6 + 0x91) != -1)) &&
         (uVar3 = *(ushort *)((short)(ushort)(byte)(*(char *)(unaff_A6 + 0x91) * '\x02') + 0xbedc0),
         unaff_D2 = CONCAT22((short)(unaff_D2 >> 0x10),uVar3), (uVar3 & 0x10) == 0)) {
        *(byte *)(unaff_A6 + 0x9a) = *(byte *)(unaff_A6 + 0x9a) & 0xdf;
        *(undefined1 *)(unaff_A6 + 0x98) = 0;
        *(undefined1 *)(unaff_A6 + 0x91) = 4;
        goto LAB_000c3a26;
      }
    }
    bVar4 = *(byte *)(unaff_A6 + 0x90);
    if (bVar4 == 0xff) goto candidate_c3b34;
    if ((*(char *)(unaff_A6 + 0x91) != -1) &&
       (unaff_D2 = CONCAT22((short)(unaff_D2 >> 0x10),
                            *(undefined2 *)
                             ((short)(ushort)(byte)(*(char *)(unaff_A6 + 0x91) * '\x02') + 0xbedc0))
       , (unaff_D2 & 1 << (bVar4 & 0x1f)) != 0)) {
      bVar4 = 0xff;
    }
    if (bVar4 == 2) {
      *(undefined1 *)(unaff_A6 + 0x98) = 0;
      *(undefined1 *)(unaff_A6 + 0x91) = 2;
    }
    else {
      if (bVar4 != 3) goto code_r0x000c380a;
      *(undefined1 *)(unaff_A6 + 0x98) = 0;
      *(undefined1 *)(unaff_A6 + 0x91) = 3;
    }
    if ((*(char *)(unaff_A6 + 0x92) != '\0') && (-1 < *(char *)(unaff_A6 + 0x92))) {
      *(undefined1 *)(unaff_A6 + 0x92) = 0;
      *(undefined1 *)(unaff_A6 + 0x91) = 5;
    }
    candidate_c0f62();
  } while( true );
LAB_000c3cd0:
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
        uVar2 = candidate_c0fea();
        if ((uVar2 & 1) == 0) {
          if ((uVar2 & 2) == 0) {
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
              bVar4 = *(byte *)(unaff_A6 + 0x9b);
              *(byte *)(unaff_A6 + 0x9b) = bVar4 & 0xef;
              if ((bVar4 & 0x10) == 0) {
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
          bVar4 = *(byte *)(unaff_A6 + 0x9b);
          *(byte *)(unaff_A6 + 0x9b) = bVar4 & 0xef;
          if ((bVar4 & 0x10) == 0) {
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
    }
    else {
      if ((cVar1 != '\x06') && (cVar1 != '\v')) {
        uVar2 = candidate_c0fea();
        if ((uVar2 & 1) == 0) {
          if ((uVar2 & 2) == 0) {
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
        uVar2 = candidate_c0fea();
        *(undefined1 *)(unaff_A6 + 0x92) = 0;
        if ((uVar2 & 1) != 0) {
          *(undefined1 *)(unaff_A6 + 0x92) = 1;
        }
        if ((uVar2 & 2) != 0) {
          *(undefined1 *)(unaff_A6 + 0x92) = 0xff;
        }
        candidate_c0f62();
        *(undefined1 *)(unaff_A6 + 0x76) = 0;
        *(code **)(unaff_A6 + 0x7c) = candidate_c3e66;
        candidate_c3778();
        if (unaff_D7w != 0) {
          *(code **)(unaff_A6 + 0x7c) = candidate_c38b2;
        }
        return;
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
  uVar2 = candidate_c0fea();
  if (*(char *)(unaff_A6 + 0x76) == '\0') {
    candidate_ca2ce();
    *(ushort *)(extraout_A0 + 4) = *(ushort *)(extraout_A0 + 4) | 2;
    goto LAB_000c3bb8;
  }
  *(undefined1 *)(unaff_A6 + 0x98) = 0;
  if ((uVar2 & 0x40) != 0) goto LAB_000c3bcc;
  if ((uVar2 & 0x10) == 0) {
LAB_000c3bb8:
    candidate_c3778();
    if (unaff_D7w != 0) {
      *(code **)(unaff_A6 + 0x7c) = candidate_c38b2;
    }
    return;
  }
  goto LAB_000c3cd0;
code_r0x000c380a:
  if (bVar4 == 1) {
    *(undefined1 *)(unaff_A6 + 0x98) = 0;
    *(undefined1 *)(unaff_A6 + 0x91) = 1;
    if (*(char *)(unaff_A6 + 0x92) < '\x01') {
      *(undefined1 *)(unaff_A6 + 0x92) = 0;
      uVar5 = 0xf8d8;
    }
    else {
      uVar5 = 0xfa58;
    }
    if (*(char *)(unaff_A6 + 0x93) != '\0') goto LAB_000c3a26;
  }
  else {
    if (bVar4 != 0) {
candidate_c3b34:
      candidate_c3778();
      if (unaff_D7w != 0) {
        *(undefined1 *)(unaff_A6 + 0x91) = 0xff;
      }
      return;
    }
    *(byte *)(unaff_A6 + 0x9a) = *(byte *)(unaff_A6 + 0x9a) | 0x20;
    *(undefined1 *)(unaff_A6 + 0x98) = 0;
    *(undefined1 *)(unaff_A6 + 0x91) = 0;
    if (*(char *)(unaff_A6 + 0x92) == '\0') {
      uVar5 = 0xf858;
    }
    else if (*(char *)(unaff_A6 + 0x92) < '\0') {
      uVar5 = 0xfb98;
    }
    else {
      uVar5 = 0xfa18;
    }
  }
  if ((((*(char *)(unaff_A6 + 0x92) == '\0') &&
       (bVar4 = *(byte *)((short)(*(short *)(unaff_A6 + 0x6e) +
                                 CONCAT11((char)((ushort)*(short *)(unaff_A6 + 0x70) >> 8),
                                          *(undefined1 *)
                                           (unaff_A5 + 0x471e + (int)*(short *)(unaff_A6 + 0x70))) *
                                 4) + 0xc38fc), bVar4 != 0xff)) &&
      (bVar4 <= *(byte *)(unaff_A6 + 0x95))) && ((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0)) {
    bVar6 = false;
    candidate_c0f96(uVar5);
    if (bVar6) {
      *(undefined1 *)(unaff_A6 + 0x95) = 0;
      *(byte *)(unaff_A6 + 0x9a) = *(byte *)(unaff_A6 + 0x9a) & 0xdf;
      *(undefined1 *)(unaff_A6 + 0x91) = 4;
      if (*(char *)(unaff_A6 + 0x93) != '\0') {
        *(undefined1 *)(unaff_A6 + 0x91) = 7;
      }
    }
  }
  goto LAB_000c3a26;
LAB_000c3bcc:
  *(byte *)(unaff_A6 + 0x9a) = *(byte *)(unaff_A6 + 0x9a) & 0xdf;
  *(undefined1 *)(unaff_A6 + 0x98) = 2;
  *(byte *)(unaff_A6 + 0x9b) = *(byte *)(unaff_A6 + 0x9b) & 0xbf;
  bVar6 = false;
  *(byte *)(unaff_A6 + 0x9c) = *(byte *)(unaff_A6 + 0x9c) & 0xbf;
  candidate_c487a();
  if (bVar6) {
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
