/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cdd98.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cdd98(void)

{
  byte bVar1;
  undefined2 uVar2;
  short sVar3;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x801;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x801;
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 2;
  candidate_1b04a();
  unaff_A6[0x22] = extraout_A0;
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x26) = uVar2;
  candidate_4256();
  *unaff_A6 = 0xcddf6;
  candidate_ce6c0();
  candidate_427a();
  if (((*(char *)((int)unaff_A6 + 0x86) == '\0') &&
      (*(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -1,
      *(short *)(unaff_A6 + 0x26) < 1)) &&
     (*unaff_A6 = candidate_cde76, *(char *)((int)unaff_A6 + 0x72) < '\x01')) {
    *unaff_A6 = 0xcde86;
  }
  candidate_2eb4();
  uVar2 = candidate_d9360();
  *(undefined2 *)(unaff_A6 + 0x27) = uVar2;
  sVar3 = -1;
  if (*(ushort *)(unaff_A6 + 0x27) != *(ushort *)((int)unaff_A6 + 0x6e)) {
    if (*(ushort *)((int)unaff_A6 + 0x6e) <= *(ushort *)(unaff_A6 + 0x27)) {
      sVar3 = 1;
    }
    *(short *)((int)unaff_A6 + 0x6e) = sVar3 + *(short *)((int)unaff_A6 + 0x6e);
  }
  bVar1 = *(byte *)(unaff_A6[3] + 0xa0);
  if (bVar1 == 2) {
    *unaff_A6 = candidate_4188;
  }
  bVar4 = bVar1 < 2;
  candidate_4c24();
  if (bVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *unaff_A6 = candidate_cdef6;
  }
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_cdf54;
  }
  return;
}
