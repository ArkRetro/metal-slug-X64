/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bcfa4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_bcfa4(void)

{
  code *pcVar1;
  int iVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xfff0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xfff0;
  candidate_4256();
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  *unaff_A6 = 0xbd044;
  *(undefined1 *)((int)unaff_A6 + 0x6e) = uRam0010c714;
  uVar3 = 0;
  candidate_1abc8();
  candidate_1945c();
  if (((!(bool)uVar3) && (candidate_427a(), _DAT_0010e99e < 0x8b0)) &&
     (0x4efffff < (uint)unaff_A6[10])) {
    return;
  }
  candidate_4126();
  iVar2 = _DAT_001063dc;
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
