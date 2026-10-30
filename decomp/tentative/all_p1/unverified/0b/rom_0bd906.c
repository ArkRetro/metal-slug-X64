/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bd906.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_bd906(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  undefined4 *unaff_A6;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x6e) = *(undefined2 *)(unaff_A6[3] + 0x6e);
  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
  *unaff_A6 = 0xbd95e;
  sVar3 = *(short *)((int)unaff_A6 + 0x76) + 1;
  if (*(short *)(*(short *)(unaff_A6 + 0x1d) + 0xbda12) <= sVar3) {
    sVar3 = 0;
    *(ushort *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + 4U & 0x1c;
  }
  *(short *)((int)unaff_A6 + 0x76) = sVar3;
  candidate_1ad6e();
  candidate_1abc8();
  candidate_140c0();
  candidate_427a();
  if (*(ushort *)((int)unaff_A6 + 0x6e) <= _DAT_0010e99e) {
    candidate_4126();
    iVar2 = _DAT_001063dc;
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
