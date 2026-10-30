/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01a134.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined1 candidate_1a134(void)

{
  char *pcVar1;
  byte bVar2;
  int unaff_D7;
  int in_A0;
  undefined2 *puVar3;
  undefined2 *puVar4;
  int unaff_A5;
  int in_stack_00000000;

  bVar2 = *(byte *)(in_stack_00000000 + 1);
  puVar3 = (undefined2 *)(in_A0 + 0x6e);
  for (puVar4 = (undefined2 *)(in_stack_00000000 + 10);
      puVar4 < (undefined2 *)(in_stack_00000000 + (short)(ushort)bVar2); puVar4 = puVar4 + 1) {
    *puVar3 = *puVar4;
    puVar3 = puVar3 + 1;
  }
  if ((-1 < unaff_D7) && (bVar2 = *(byte *)(in_stack_00000000 + 2), bVar2 < 9)) {
    *(byte *)(in_A0 + 0x69) = bVar2;
    pcVar1 = (char *)(unaff_A5 + 0x4326 + (int)(short)(ushort)bVar2);
    *pcVar1 = *pcVar1 + '\x01';
  }
  *(byte *)(in_A0 + 0x68) = *(byte *)(in_stack_00000000 + 3) & 7;
  candidate_1abb6();
  return *(undefined1 *)(in_stack_00000000 + 1);
}
