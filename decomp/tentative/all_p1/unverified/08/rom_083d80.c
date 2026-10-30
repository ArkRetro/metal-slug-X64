/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x083d80.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_83d80(void)

{
  code *pcVar1;
  int iVar2;
  short extraout_D0u;
  ushort uVar3;
  short sVar4;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;

  candidate_84e2c();
  candidate_427a();
  unaff_A6[0xc] = (*(int *)((int)unaff_A6 + 0x6e) - unaff_A6[0xc] >> 3) + unaff_A6[0xc];
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    bVar5 = false;
    if (*(char *)((int)unaff_A6 + 0x99) == '\0') {
      candidate_1ac4c();
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
        if (extraout_D0u <= *(short *)(unaff_A6 + 0x21)) {
          *(undefined1 *)(unaff_A6 + 0x18) = 1;
          *unaff_A6 = candidate_83e2c;
        }
      }
      else if (*(short *)(unaff_A6 + 0x21) <= extraout_D0u) {
        *(undefined1 *)(unaff_A6 + 0x18) = 1;
        *unaff_A6 = candidate_83e2c;
      }
    }
    else {
      candidate_1aeda();
      if (bVar5) {
        uVar3 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0xf);
        if ((int)((uint)uVar3 << 0x10) < 0) {
          uVar3 = -uVar3;
        }
        if (uVar3 <= *(ushort *)(unaff_A6 + 0x21)) {
          *(undefined1 *)(unaff_A6 + 0x18) = 1;
          *unaff_A6 = candidate_83e2c;
        }
      }
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_8404e;
  }
  candidate_4c24();
  if (*(char *)((int)unaff_A6 + 0x5a) != '\0') {
    if (((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) &&
       (*(char *)((int)unaff_A6 + 0x5a) == '\x06')) {
      candidate_120f8();
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  sVar4 = candidate_49e8();
  if (sVar4 != 0) {
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
    *unaff_A6 = candidate_8404e;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
    *unaff_A6 = candidate_84010;
  }
  bVar5 = false;
  candidate_1945c();
  if (bVar5) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
