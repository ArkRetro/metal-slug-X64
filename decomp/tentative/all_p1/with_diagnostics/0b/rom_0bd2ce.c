/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bd2ce.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_bd2ce(void)

{
  ushort *puVar1;
  code *pcVar2;
  ushort uVar3;
  ushort uVar4;
  int iVar5;
  short sVar6;
  short unaff_D4w;
  short unaff_D7w;
  int extraout_A0;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x6e) = 2;
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0x10c70d;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0;
  *unaff_A6 = candidate_bd20e;
  if (*(short *)(*(short *)((int)unaff_A6 + 0x6e) + 0xbd2a8) == -1) {
    candidate_4126();
    iVar5 = _DAT_001063dc;
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar2 = (code *)*unaff_A6;
      *(undefined4 *)(iVar5 + -4) = 0x424a;
      (*pcVar2)();
    } while( true );
  }
  candidate_c6f6a();
  sVar6 = ((*(short *)(extraout_A0 + 0x36) + 0x1f) - unaff_D4w) * 0x10;
  if (((-1 < sVar6) && (**(char **)((int)unaff_A6 + 0x72) == '\0')) &&
     (sVar6 != *(short *)(unaff_A6 + 0x1c))) {
    pcVar2 = *(code **)((short)(*(short *)((int)unaff_A6 + 0x6e) << 1) + 0xbd2ae);
    if (pcVar2 == (code *)0xffffffff) {
      __m68k_trap(0xf);
      puVar1 = (ushort *)(unaff_D7w * 2 + 0x108069);
      uVar3 = *puVar1;
      uVar4 = uVar3 + 2;
      *puVar1 = uVar4;
      if (0xfffd < uVar3 || uVar4 == 0) {
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
    (*pcVar2)();
    **(undefined1 **)((int)unaff_A6 + 0x72) = 1;
  }
  *(short *)(unaff_A6 + 0x1c) = sVar6;
  return;
}
