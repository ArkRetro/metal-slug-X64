/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b91fa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_b91fa(void)

{
  code *pcVar1;
  int iVar2;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xfff1;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xfff1;
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  candidate_4256();
  *unaff_A6 = 0xb9236;
  candidate_1abc8();
  candidate_427a();
  if (0xf7f < _DAT_0010e99e) {
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
