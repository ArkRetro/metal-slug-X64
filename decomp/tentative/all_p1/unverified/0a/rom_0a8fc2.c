/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a8fc2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a8fc2(void)

{
  short sVar1;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x22) = 0;
  *unaff_A6 = 0xa9010;
  uVar2 = 0;
  if (*(char *)((int)unaff_A6 + 0x95) == '\0') {
    candidate_1436a();
  }
  else {
    candidate_1abc8();
  }
  candidate_427a();
  if ((bool)uVar2) {
    uVar2 = 0;
    candidate_4256();
  }
  candidate_a9b1c();
  if ((bool)uVar2) {
    if (*(short *)(unaff_A6 + 0x20) == 0) {
      if (*(char *)(unaff_A6 + 0x25) == '\0') {
        *(undefined2 *)(unaff_A6 + 0x24) = 0x28;
        *(undefined1 *)(unaff_A6 + 0x25) = 0xff;
        uVar2 = 0;
      }
      else if (*(short *)(unaff_A6 + 0x24) == 0) {
        *unaff_A6 = 0xa935a;
        uVar2 = 0;
      }
      else {
        sVar1 = *(short *)(unaff_A6 + 0x24);
        uVar2 = sVar1 == 0;
        *(short *)(unaff_A6 + 0x24) = sVar1 + -1;
      }
    }
    else if (*(short *)(unaff_A6 + 0x20) == 1) {
      unaff_A6[0x14] = 0xe64ec;
      uVar2 = 0;
    }
    else {
      uVar2 = 0;
      if (*(char *)(unaff_A6 + 0x25) == '\0') {
        candidate_40bc();
        if (0xa0 < *(short *)(unaff_A6 + 0xf)) {
          *(byte *)(extraout_A0 + 0x15) = *(byte *)(extraout_A0 + 0x15) | 1;
        }
        candidate_1acf2();
        *(undefined1 *)(unaff_A6 + 0x25) = 0xff;
        uVar2 = 0;
      }
    }
  }
  else {
    candidate_a9b60();
    if ((bool)uVar2) {
      *(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + 1;
      uVar2 = *(ushort *)(unaff_A6 + 0x22) < *(ushort *)(unaff_A6 + 0x21);
      if (((short)*(ushort *)(unaff_A6 + 0x21) <= (short)*(ushort *)(unaff_A6 + 0x22)) &&
         (candidate_1af9a(), !(bool)uVar2)) {
        bVar3 = false;
        candidate_a9c18();
        if (bVar3) {
          *unaff_A6 = candidate_a914a;
          uVar2 = 0;
        }
        else {
          *unaff_A6 = candidate_a927e;
          uVar2 = 0;
        }
      }
    }
    else {
      *(undefined2 *)(unaff_A6 + 0x22) = 0;
      uVar2 = 0;
    }
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_a93dc;
  }
  candidate_a9a56();
  return;
}
