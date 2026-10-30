/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09eccc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9eccc(void)

{
  ushort uVar1;
  short sVar2;
  char cVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  uVar1 = candidate_260c8();
  sVar2 = (uVar1 & 7) << 3;
  unaff_A6[0xd] = *(undefined4 *)(sVar2 + 0x2d253c);
  *(undefined4 *)((int)unaff_A6 + 0x72) = *(undefined4 *)(sVar2 + 0x2d2540);
  sVar2 = *(short *)((short)((uVar1 & 7) * 2) + 0x2d252c);
  if (sVar2 == 0) {
    sVar2 = 1;
  }
  unaff_A6[0xc] = (short)(((int)*(short *)(unaff_A6[3] + 0x86) << 8) / (int)sVar2) * 0xa0;
  candidate_9ea8();
  candidate_9ea8();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_4256();
  *unaff_A6 = 0x9ed76;
  func_0x000143e4();
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x72) + unaff_A6[0xd];
  if ((0x1cf < *(short *)(unaff_A6 + 0x10)) && (-1 < (int)unaff_A6[0xd])) {
    unaff_A6[0xd] = 0;
  }
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_9edf0;
  }
  sVar2 = candidate_49e8();
  if (sVar2 != 0) {
    *unaff_A6 = candidate_41ac;
  }
  *(undefined1 *)(unaff_A6 + 5) =
       *(undefined1 *)((int)unaff_A6 + (int)(short)((*(byte *)(unaff_A6 + 0x25) & 3) + 0x19));
  cVar3 = *(byte *)(unaff_A6 + 0x25) + 1;
  if ('\x02' < cVar3) {
    cVar3 = '\0';
  }
  *(char *)(unaff_A6 + 0x25) = cVar3;
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_9ec34;
  }
  bVar4 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_9ec34;
  }
  return;
}
