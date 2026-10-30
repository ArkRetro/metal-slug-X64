/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bd84a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_bd84a(void)

{
  code *pcVar1;
  short sVar2;
  int iVar3;
  int extraout_A0;
  undefined4 *unaff_A6;

  candidate_40bc();
  *(undefined2 *)(extraout_A0 + 0x74) = *(undefined2 *)(unaff_A6 + 0x1d);
  *(undefined4 *)(extraout_A0 + 0x38) = 0x10e968;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0x10000;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0x340;
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0;
  *unaff_A6 = 0xbd898;
  candidate_bd9d4();
  sVar2 = *(short *)(*(short *)((int)unaff_A6 + 0x72) + 0xbda0c);
  if (sVar2 == *(short *)(unaff_A6 + 0xb)) {
    unaff_A6[0xd] = 0;
  }
  else if (*(short *)(unaff_A6 + 0xb) < sVar2) {
    unaff_A6[0xd] = 0x10000;
  }
  else {
    unaff_A6[0xd] = 0xffff0000;
  }
  candidate_1ac38();
  if (*(ushort *)((int)unaff_A6 + 0x6e) <= _DAT_0010e99e) {
    candidate_4126();
    iVar3 = _DAT_001063dc;
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar3 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
