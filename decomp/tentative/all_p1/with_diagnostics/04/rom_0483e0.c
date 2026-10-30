/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0483e0.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x00048473) */

void candidate_483e0(void)

{
  char cVar1;
  undefined2 uVar2;
  undefined1 uVar3;
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar4;

  if (*(short *)(unaff_A6 + 0x1e) < 0xa1) {
    *(undefined2 *)(unaff_A6 + 0x1e) = 0x120;
  }
  else {
    *(undefined2 *)(unaff_A6 + 0x1e) = 0x20;
  }
  if (0x13 < *(ushort *)((int)unaff_A6 + 0x6e)) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  *(ushort *)((int)unaff_A6 + 0x7a) = *(ushort *)((int)unaff_A6 + 0x6e) * 0x10 + 0x100;
  *(undefined1 *)((int)unaff_A6 + 0x96) = 7;
  candidate_4256();
  *(undefined1 *)(unaff_A6[0x1f] + 0x60) = 0;
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar2;
  *(undefined1 *)(unaff_A6 + 0x2a) = 0;
  *(undefined2 *)(unaff_A6 + 0x24) = 0;
  *unaff_A6 = 0x48462;
  candidate_49f9e();
  func_0x000143e4();
  candidate_49b20();
                    /* WARNING: Bad instruction - Truncating control flow here */
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x9a) == '\0') {
    if (*(char *)((int)unaff_A6 + 0x97) == '\0') {
      uVar3 = candidate_260c8();
      *(undefined1 *)((int)unaff_A6 + 0x97) = uVar3;
      *(ushort *)(unaff_A6 + 0x20) = (ushort)*(byte *)((int)unaff_A6 + 0x96) * 8 + 0xa0;
      candidate_40bc();
      *(undefined1 *)(extraout_A0 + 0x60) = 0;
      cVar1 = *(char *)(unaff_A6 + 0x26) + -1;
      *(char *)(unaff_A6 + 0x26) = cVar1;
      if (cVar1 == '\0') {
        uVar3 = candidate_260c8();
        *(undefined1 *)(unaff_A6 + 0x26) = uVar3;
        uVar3 = candidate_260c8();
        *(undefined1 *)((int)unaff_A6 + 0x9a) = uVar3;
      }
    }
    else {
      *(char *)((int)unaff_A6 + 0x97) = *(char *)((int)unaff_A6 + 0x97) + -1;
    }
  }
  else {
    *(char *)((int)unaff_A6 + 0x9a) = *(char *)((int)unaff_A6 + 0x9a) + -1;
  }
  *(char *)((int)unaff_A6 + 0x96) = *(char *)((int)unaff_A6 + 0x96) + '\x03';
  bVar4 = false;
  candidate_49e74();
  if (bVar4) {
    *unaff_A6 = candidate_48060;
  }
  candidate_4900c();
  return;
}
