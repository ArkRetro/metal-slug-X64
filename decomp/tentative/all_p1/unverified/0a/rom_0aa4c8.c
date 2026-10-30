/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa4c8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aa4c8(void)

{
  code *pcVar1;
  undefined2 *puVar2;
  int unaff_A5;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x70) == 0) {
    puVar2 = (undefined2 *)(unaff_A5 + 0x4608);
    pcVar1 = candidate_42e8;
  }
  else {
    puVar2 = (undefined2 *)(unaff_A5 + 0x462c);
    pcVar1 = thunk_FUN_000044bc;
  }
  *puVar2 = 0;
  *(undefined1 *)(puVar2 + 0xe) = 2;
  *(undefined4 *)(pcVar1 + unaff_A5) = 0x16dbe;
  return;
}
