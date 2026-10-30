/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05287c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_5287c(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  DAT_0010c44a = 0;
  candidate_1ab98();
  candidate_1abc8();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xffff;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xffff;
  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x1c) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0;
  *(undefined1 *)(unaff_A6 + 0x1d) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x75) = 0;
  *unaff_A6 = 0x528de;
  *(short *)(unaff_A6 + 0x1c) = *(short *)(unaff_A6 + 0x1c) + 1;
  sVar1 = _DAT_0010e99e;
  if (_DAT_0010e99e != *(short *)((int)unaff_A6 + 0x72)) {
    *(undefined2 *)(unaff_A6 + 0x1c) = 0;
  }
  *(short *)((int)unaff_A6 + 0x72) = sVar1;
  uVar2 = false;
  candidate_c7b26();
  if (!(bool)uVar2) {
    sVar1 = candidate_19de4();
    uVar2 = false;
    if (sVar1 == 0) {
      candidate_18f24();
      if (!(bool)uVar2) {
        candidate_19424();
        if (!(bool)uVar2) {
          bVar3 = false;
          candidate_1aeb0();
          if (!bVar3) {
            uVar2 = false;
            candidate_1aeb0();
            if (!(bool)uVar2) goto LAB_0005299c;
          }
          uVar2 = *(ushort *)(unaff_A6 + 0x1c) < 0xd2;
          if (!(bool)uVar2) {
            uVar2 = 0;
            if (*(char *)(unaff_A6 + 0x1d) == '\0') {
              uVar2 = 0;
              candidate_4256();
              *(undefined1 *)(unaff_A6 + 0x1d) = 0xff;
            }
            candidate_427a();
            if ((bool)uVar2) {
              *(undefined1 *)(unaff_A6 + 0x1d) = 0;
              *(char *)((int)unaff_A6 + 0x75) = *(char *)((int)unaff_A6 + 0x75) + '\x01';
              if (1 < *(byte *)((int)unaff_A6 + 0x75)) {
                *(undefined2 *)(unaff_A6 + 0x1c) = 0x96;
                *(undefined1 *)((int)unaff_A6 + 0x75) = 0;
              }
            }
            return;
          }
        }
      }
    }
  }
LAB_0005299c:
  candidate_427a();
  if ((bool)uVar2) {
    candidate_4256();
  }
  *(undefined1 *)(unaff_A6 + 0x1d) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x75) = 0;
  return;
}
